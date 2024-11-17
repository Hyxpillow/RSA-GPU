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

void OBN_mul_gpu(OURBIGNUM *r, const OURBIGNUM *a, const OURBIGNUM *b, const OBN_MUL_GPU_CTX *ctx)
{
    cudaMemcpy(&(ctx->ad), a, sizeof(OURBIGNUM), cudaMemcpyHostToDevice);
    cudaMemcpy(&(ctx->bd), b, sizeof(OURBIGNUM), cudaMemcpyHostToDevice);
    cudaMemset(&(ctx->rd), 0, sizeof(OURBIGNUM));
    cudaMemset(ctx->buf, 0, sizeof(OURBIGNUM) * sizeof(OURBIGNUM));

    dim3 blockSize(16, 16);
    dim3 gridSize(8, 8);
    f_kernel<<<gridSize, blockSize>>>(ctx->buf, &(ctx->ad), &(ctx->bd));

    for (size_t i = 0; i < OBN_MAX_NUM_BYTES; ++i)
    {
        unsigned int carry = 0;
        for (size_t j = 0; j < OBN_MAX_NUM_BYTES - i; ++j)
        {
            unsigned int product =
                r->data[i + j] +
                buf[i][j]
                carry;

            r->data[i + j] = product & 0xFF;
            carry = product >> 8;
        }
    }

    cudaMemcpy(r, &(ctx->buf3), sizeof(OURBIGNUM), cudaMemcpyHostToDevice);
}

__global__ void f_kernel(unsigned int **buf, const OURBIGNUM *ad, const OURBIGNUM *bd)
{
    int xxx = 1 + blockIdx.x * blockDim.x + threadIdx.x;
    int yyy = 1 + blockIdx.y * blockDim.y + threadIdx.y;

    for(int i = xxx; i < N - 1; i += gridDim.x * blockDim.x)
    {
        for(int j = yyy; j < N - 1; j += gridDim.y * blockDim.y)
        {
            buf[i][j] = ad->data[i] * bd->data[j];
        }
    }
}

