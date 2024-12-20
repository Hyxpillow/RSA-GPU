#include "modexp_cpu_mont.h"
#include <iostream>

void REDC(OURBIGNUM* T, OURBIGNUM* result, const BN_CONFIG& bn_config) {
    OURBIGNUM *tmp = OBN_new();
    OBN_copy(tmp, T);

    OURBIGNUM* m = OBN_new();
    OURBIGNUM* t = OBN_new();
    OBN_mask_bits(tmp, bn_config.k);
    OBN_mod_mul(m, tmp, bn_config.N_, bn_config.R);

     
    OBN_mul(tmp, m, bn_config.N);
    OBN_add(t, T, tmp);
    OBN_rshift(tmp, t, bn_config.k);
    OBN_copy(t, tmp);

    if (OBN_cmp(t, bn_config.N) >= 0) {
        OBN_sub(result, t, bn_config.N);
    } else {
        OBN_copy(result, t);
    }

    OBN_free(tmp);
    OBN_free(m);
    OBN_free(t);
}

void toMontgomeryForm(const OURBIGNUM* a, OURBIGNUM* result, const BN_CONFIG& bn_config) 
{
     
    OURBIGNUM *tmp = OBN_new();
    OBN_mod_mul(tmp, a, bn_config.R2, bn_config.N);
    REDC(tmp, result, bn_config);
    OBN_free(tmp);
}

void fromMontgomeryForm(const OURBIGNUM* a, OURBIGNUM* result, const BN_CONFIG& bn_config) 
{
    OURBIGNUM *tmp = OBN_new();
    OBN_copy(tmp, a);
    REDC(tmp, result, bn_config);
    OBN_free(tmp);
}

void mont_mul(const OURBIGNUM* a, const OURBIGNUM* b, OURBIGNUM* result, const BN_CONFIG& bn_config) 
{
    OURBIGNUM *T = OBN_new();
    OBN_mul(T, a, b);
    REDC(T, result, bn_config);
}

void mont_mod_exp(OURBIGNUM* r, 
                  const OURBIGNUM* a, 
                  const OURBIGNUM* p, 
                  const BN_CONFIG& bn_config) 
{       
    OURBIGNUM *mont_base = OBN_new();
    toMontgomeryForm(a, mont_base, bn_config);
     
    OURBIGNUM *mont_result = OBN_new();
    OBN_one(mont_result);
    toMontgomeryForm(mont_result, mont_result, bn_config);
     
    for (int i = OBN_num_bits(p) - 1; i >= 0; i--)
    {
         
        mont_mul(mont_result, mont_result, mont_result, bn_config);

         
        if (OBN_is_bit_set(p, i))
        {
            mont_mul(mont_result, mont_base, mont_result, bn_config);
        }
    }

     
    fromMontgomeryForm(mont_result, r, bn_config);
    OBN_free(mont_base);
    OBN_free(mont_result);
}