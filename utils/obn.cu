#include "obn.h"
#include "cuda.h"

OBN_MUL_GPU_CTX* OBN_MUL_GPU_CTX_new()
{
    OBN_MUL_GPU_CTX *p;
    cudaMalloc(&p, sizeof(OBN_MUL_GPU_CTX));
    return p;
}

void OBN_MUL_GPU_CTX_free(OBN_MUL_GPU_CTX* p)
{
    cudaFree(p);
}

__global__ void multiply_kernel(int **buf, const OURBIGNUM *ad, const OURBIGNUM *bd)
{
    int xxx = 1 + blockIdx.x * blockDim.x + threadIdx.x;
    int yyy = 1 + blockIdx.y * blockDim.y + threadIdx.y;

    for(int i = xxx; i < OBN_MAX_NUM_BYTES - 1; i += gridDim.x * blockDim.x)
    {
        for(int j = yyy; j < OBN_MAX_NUM_BYTES - 1; j += gridDim.y * blockDim.y)
        {
            buf[i][j] = ad->data[i] * bd->data[j];
        }
    }
}

void OBN_mul_gpu(OURBIGNUM *r, const OURBIGNUM *a, const OURBIGNUM *b, OBN_MUL_GPU_CTX *ctx)
{
    cudaMemcpy((void*)(ctx->ad.data), a, sizeof(OURBIGNUM), cudaMemcpyHostToDevice);
    cudaMemcpy((void*)(ctx->bd.data), b, sizeof(OURBIGNUM), cudaMemcpyHostToDevice);
    cudaMemset((void*)(ctx->buf), 0, sizeof(OURBIGNUM) * sizeof(OURBIGNUM));

    memset(r->data, 0, sizeof(OURBIGNUM));

    dim3 blockSize(16, 16);
    dim3 gridSize(8, 8);
    multiply_kernel<<<gridSize, blockSize>>>(ctx->buf, &(ctx->ad), &(ctx->bd));

    for (size_t i = 0; i < OBN_MAX_NUM_BYTES; ++i)
    {
        unsigned int carry = 0;
        for (size_t j = 0; j < OBN_MAX_NUM_BYTES - i; ++j)
        {
            unsigned int product =
                r->data[i + j] +
                ctx->buf[i][j] +
                carry;

            r->data[i + j] = product & 0xFF;
            carry = product >> 8;
        }
    }
}

