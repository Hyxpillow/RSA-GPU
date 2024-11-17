#include "obn.h"

void gpu_mod_exp(OURBIGNUM* r, const OURBIGNUM* a, const OURBIGNUM* p, const OURBIGNUM* m, OBN_MUL_GPU_CTX *ctx)
{
    OBN_one(r);
    OURBIGNUM *xxx = OBN_new();
    OBN_copy(xxx, a);
    OURBIGNUM *tmp = OBN_new();

    int num_bits_exp = OBN_num_bits(p);
    for (int i = 0; i < num_bits_exp; i++) {
        if (OBN_is_bit_set(p, i)) {
            OBN_mul_gpu(tmp, r, xxx, ctx);
            OBN_mod(NULL, r, tmp, m);
        }
        OBN_mul_gpu(tmp, xxx, xxx, ctx);
        OBN_mod(NULL, xxx, tmp, m);
    }

    OBN_free(xxx);
    OBN_free(tmp);

    OBN_MUL_GPU_CTX_free(ctx);
}