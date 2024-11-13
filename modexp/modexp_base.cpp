#include "utils/obn.h"

void base_mod_exp(OURBIGNUM* r, const OURBIGNUM* a, const OURBIGNUM* p, const OURBIGNUM* m)
{
    OBN_one(r);
    OURBIGNUM *xxx = OBN_new();
    OBN_copy(xxx, a);

    int num_bits_exp = OBN_num_bits(p);
    for (int i = 0; i < num_bits_exp; i++) {
        if (OBN_is_bit_set(p, i)) {
            OBN_mod_mul(r, r, xxx, m);
        }
        OBN_mod_mul(xxx, xxx, xxx, m);
    }

    OBN_free(xxx);
}