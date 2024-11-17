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

__global__ void multiply_kernel(int buf[OBN_MAX_NUM_BYTES][OBN_MAX_NUM_BYTES], const OURBIGNUM *ad, const OURBIGNUM *bd)
{
    int xxx = blockIdx.x * blockDim.x + threadIdx.x;
    int yyy = blockIdx.y * blockDim.y + threadIdx.y;

    for (int i = xxx; i < OBN_MAX_NUM_BYTES; i += gridDim.x * blockDim.x)
    {
        for (int j = yyy; j < OBN_MAX_NUM_BYTES; j += gridDim.y * blockDim.y)
        {
            buf[i][j] = ad->data[i] * bd->data[j];
        }
    }
}

void OBN_mul_gpu(OURBIGNUM *r, const OURBIGNUM *a, const OURBIGNUM *b, OBN_MUL_GPU_CTX *ctx)
{
    int buf[OBN_MAX_NUM_BYTES][OBN_MAX_NUM_BYTES];
    cudaMemcpy((void *)(ctx->ad.data), a, sizeof(OURBIGNUM), cudaMemcpyHostToDevice);
    cudaMemcpy((void *)(ctx->bd.data), b, sizeof(OURBIGNUM), cudaMemcpyHostToDevice);
    cudaMemset((void *)(ctx->buf), 0, sizeof(int) * OBN_MAX_NUM_BYTES * OBN_MAX_NUM_BYTES);

    dim3 blockSize(16, 16);
    dim3 gridSize(8, 8);
    multiply_kernel<<<gridSize, blockSize>>>(ctx->buf, &(ctx->ad), &(ctx->bd));
    cudaDeviceSynchronize();
    cudaMemcpy((void *)buf,
               (void *)(ctx->buf),
               sizeof(int) * OBN_MAX_NUM_BYTES * OBN_MAX_NUM_BYTES,
               cudaMemcpyDeviceToHost);

    memset(r->data, 0, sizeof(OURBIGNUM));
    for (size_t i = 0; i < OBN_MAX_NUM_BYTES; ++i)
    {
        unsigned int carry = 0;
        for (size_t j = 0; j < OBN_MAX_NUM_BYTES - i; ++j)
        {
            unsigned int product = r->data[i + j] + buf[i][j] + carry;
            r->data[i + j] = product & 0xFF;
            carry = product >> 8;
        }
    }
}
