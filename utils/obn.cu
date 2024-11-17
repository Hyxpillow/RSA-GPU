#include "obn.h"
#include "cuda.h"

OBN_MUL_GPU_CTX *OBN_MUL_GPU_CTX_new()
{
    OBN_MUL_GPU_CTX *p;
    cudaMalloc(&p, sizeof(OBN_MUL_GPU_CTX));
    return p;
}

void OBN_MUL_GPU_CTX_free(OBN_MUL_GPU_CTX *p)
{
    cudaFree(p);
}

__global__ void multiply_kernel(OBN_MUL_GPU_CTX* ctx)
{
    int xxx = blockIdx.x * blockDim.x + threadIdx.x;
    int yyy = blockIdx.y * blockDim.y + threadIdx.y;

    for (int i = xxx; i < OBN_MAX_NUM_BYTES; i += gridDim.x * blockDim.x)
    {
        for (int j = yyy; j < OBN_MAX_NUM_BYTES - i; j += gridDim.y * blockDim.y)
        {
            atomicAdd(&(ctx->buf[i + j]), (int)ctx->a[i] * (int)ctx->b[j]);
        }
    }
}

__global__ void split_int32_kernel(OBN_MUL_GPU_CTX* ctx)
{
    int idx = blockIdx.x * blockDim.x + threadIdx.x;
    for (int i = idx; i < OBN_MAX_NUM_BYTES; i += gridDim.x * blockDim.x)
    {
        ctx->low8[i] = ctx->buf[i]  & 0xFF;
        ctx->mid8[i] = (ctx->buf[i] >> 8) & 0xFF;
        ctx->high8[i] = (ctx->buf[i] >> 16) & 0xFF;
    }
}

__global__ void is_carry_all_zero(OBN_MUL_GPU_CTX* ctx)
{
    int idx = blockIdx.x * blockDim.x + threadIdx.x;
    for (int i = idx; i < OBN_MAX_NUM_BYTES; i += gridDim.x * blockDim.x)
        if (ctx->carry[i] != 0)
            ctx->carry_zero_flag = 1; 
}

__global__ void add_kernel(OBN_MUL_GPU_CTX* ctx)
{
    int idx = blockIdx.x * blockDim.x + threadIdx.x;
    for (int i = idx; i < OBN_MAX_NUM_BYTES; i += gridDim.x * blockDim.x)
    {
        int tmp = (int)ctx->x[i] + (int)ctx->y[i];
        ctx->z[i] = tmp & 0xFF;
        if (i + 1 < OBN_MAX_NUM_BYTES)
        {
            ctx->carry[i + 1] = (tmp >> 8) & 0xFF;
        } else {
            ctx->carry[0] = 0;
        }
    }
}

void OBN_mul_gpu(OURBIGNUM *r, const OURBIGNUM *a, const OURBIGNUM *b, OBN_MUL_GPU_CTX *ctx)
{
    cudaMemcpy((void *)(ctx->a), a, OBN_MAX_NUM_BYTES, cudaMemcpyHostToDevice);
    cudaMemcpy((void *)(ctx->b), b, OBN_MAX_NUM_BYTES, cudaMemcpyHostToDevice);
    cudaMemset((void *)(ctx->buf), 0, sizeof(int) * OBN_MAX_NUM_BYTES);

    int carry_zero_flag;

    dim3 blockSize(16, 16);
    dim3 gridSize(8, 8);
    multiply_kernel<<<gridSize, blockSize>>>(ctx);
    cudaDeviceSynchronize();

    // get base, carry8, carry16
    split_int32_kernel<<<8, 32>>>(ctx);
    cudaDeviceSynchronize();

    // if carry8 != 0
    cudaMemcpy((void *)(ctx->carry), ctx->mid8, OBN_MAX_NUM_BYTES, cudaMemcpyDeviceToDevice);
    is_carry_all_zero<<<8, 32>>>(ctx);
    cudaDeviceSynchronize();
    cudaMemcpy(&carry_zero_flag, (void *)&(ctx->carry_zero_flag), sizeof(int), cudaMemcpyDeviceToHost);
    while (carry_zero_flag)
    {
        cudaMemcpy((void *)(ctx->x), ctx->low8, OBN_MAX_NUM_BYTES, cudaMemcpyDeviceToDevice);
        cudaMemcpy((void *)(ctx->y), ctx->carry, OBN_MAX_NUM_BYTES, cudaMemcpyDeviceToDevice);

        add_kernel<<<8, 32>>>(ctx);
        cudaDeviceSynchronize();

        cudaMemcpy((void *)(ctx->low8), ctx->z, OBN_MAX_NUM_BYTES, cudaMemcpyDeviceToDevice);

        is_carry_all_zero<<<8, 32>>>(ctx);
        cudaDeviceSynchronize();
        cudaMemcpy(&carry_zero_flag, (void *)&(ctx->carry_zero_flag), sizeof(int), cudaMemcpyDeviceToHost);
    }
    
    // if carry16 != 0
    cudaMemcpy((void *)(ctx->carry), ctx->high8, OBN_MAX_NUM_BYTES, cudaMemcpyDeviceToDevice);
    is_carry_all_zero<<<8, 32>>>(ctx);
    cudaDeviceSynchronize();
    cudaMemcpy(&carry_zero_flag, (void *)&(ctx->carry_zero_flag), sizeof(int), cudaMemcpyDeviceToHost);
    while (ctx->carry_zero_flag)
    {
        cudaMemcpy((void *)(ctx->x), ctx->low8, OBN_MAX_NUM_BYTES, cudaMemcpyDeviceToDevice);
        cudaMemcpy((void *)(ctx->y), ctx->carry, OBN_MAX_NUM_BYTES, cudaMemcpyDeviceToDevice);

        add_kernel<<<8, 32>>>(ctx);
        cudaDeviceSynchronize();

        cudaMemcpy((void *)(ctx->low8), ctx->z, OBN_MAX_NUM_BYTES, cudaMemcpyDeviceToDevice);

        is_carry_all_zero<<<8, 32>>>(ctx);
        cudaDeviceSynchronize();
        cudaMemcpy(&carry_zero_flag, (void *)&(ctx->carry_zero_flag), sizeof(int), cudaMemcpyDeviceToHost);
    }

    cudaMemcpy((void *)r, (void *)(ctx->result), OBN_MAX_NUM_BYTES, cudaMemcpyDeviceToHost);
}
