#include "utils/config.h"
#include "utils/obn.h"
#include <iostream>

void REDC(OURBIGNUM& T, OURBIGNUM& result, const BN_CONFIG& bn_config) {
    OURBIGNUM tmp = T;
    OURBIGNUM m;
    OURBIGNUM t;
    OBN_mask_bits(&tmp, bn_config.k);
    OBN_mod_mul(&m, &tmp, bn_config.N_, bn_config.R);

    // t = (T + mN) / R
    OBN_mul(&tmp, &m, &bn_config.N);
    OBN_add(&t, &T, &tmp);
    OBN_rshift(&t, &t, bn_config.k);

    if (OBN_cmp(&t, &bn_config.N) >= 0) {
        OBN_sub(&result, &t, &bn_config.N);
    } else {
        OBN_copy(&result, &t);
    }
}

void toMontgomeryForm(const OURBIGNUM& a, OURBIGNUM& result, const BN_CONFIG& bn_config) 
{
    // result = a * R mod N
    OURBIGNUM tmp;
    OBN_mod_mul(&tmp, &a, &bn_config.R, &bn_config.N);
    REDC(tmp, result, bn_config);
}

void fromMontgomeryForm(const OURBIGNUM& a, OURBIGNUM& result, const BN_CONFIG& bn_config) 
{
    // result = a * 1 mod N
    OURBIGNUM one;
    OBN_set_byte(&one, 1);
    OURBIGNUM tmp;
    OBN_mul(&tmp, &a, &one);
    REDC(tmp, result, bn_config);
}

void mont_mul(const OURBIGNUM& a, const OURBIGNUM& b, OURBIGNUM& result, const BN_CONFIG& bn_config) 
{
    OURBIGNUM T;
    OBN_mul(&T, &a, &b);
    REDC(T, result, bn_config);
}

void mont_mod_exp(const OURBIGNUM& base, 
                  const OURBIGNUM& exp, 
                  OURBIGNUM& result, 
                  const BN_CONFIG& bn_config) 
{
        // 转换底数到Montgomery域
        // BIGNUM *mont_base = BN_new();
        OURBIGNUM mont_base;
        toMontgomeryForm(base, mont_base, bn_config);

        // 初始化结果为1的Montgomery形式
        // BIGNUM *mont_result = BN_new();
        // BN_one(mont_result);
        OURBIGNUM mont_result = 1;
        toMontgomeryForm(mont_result, mont_result, bn_config);

        // 按位进行幂运算
        for (int i = BN_num_bits(exp) - 1; i >= 0; i--)
        {
            // 平方
            mont_mul(mont_result, mont_result, mont_result, bn_config);

            // 如果当前位为1，乘以底数
            if (BN_is_bit_set(exp, i))
            {
                mont_mul(mont_result, mont_base, mont_result, bn_config);
            }
        }

        // 转换结果回普通域
        fromMontgomeryForm(mont_result, result, bn_config);
}