#include "modexp.h"
#include <iostream>
#include <cassert>

ModExp::ModExp() {
    ctx = BN_CTX_new();
    if (!ctx) {
        throw std::runtime_error("Failed to create BN_CTX");
    }
}

ModExp::~ModExp() {
    if (ctx) BN_CTX_free(ctx);
}

void ModExp::mod_exp(const BIGNUM* base, const BIGNUM* exp, const BIGNUM* n, BIGNUM* result) {
    BIGNUM *temp = BN_new();
    BIGNUM *square = BN_new();
    if (!temp || !square) {
        BN_free(temp);
        BN_free(square);
        throw std::runtime_error("Failed to allocate BIGNUM");
    }

    BN_one(result);
    
    for (int i = BN_num_bits(exp) - 1; i >= 0; i--) {
        BN_mod_mul(square, result, result, n, ctx);
        BN_copy(result, square);

        if (BN_is_bit_set(exp, i)) {
            BN_mod_mul(temp, result, base, n, ctx);
            BN_copy(result, temp);
        }
    }

    BN_free(temp);
    BN_free(square);
}

// MontgomeryModExp 实现
MontgomeryModExp::MontgomeryModExp(const BIGNUM* modulus) {
        ctx = BN_CTX_new();
        if (ctx == nullptr)
        {
            throw std::runtime_error("Failed to create BN_CTX");
        }

        // 分配内存
        N = BN_new();
        R = BN_new();
        R2 = BN_new();
        N_ = BN_new();

        if (!N || !R || !R2 || !N_)
        {
            throw std::runtime_error("Failed to allocate BIGNUM");
        }

        // 复制模数
        BN_copy(N, modulus);

        // 计算k：N的位长
        k = BN_num_bits(N);
        k = ((k + 31) / 32) * 32;

        // 计算R = 2^k
        BN_set_bit(R, k);

        // 计算R2 mod N
        BN_mod_mul(R2, R, R, N, ctx);

        // 计算N' = -N^(-1) mod R
        computeNPrime();
}

MontgomeryModExp::~MontgomeryModExp() {
    cleanup();
}

void MontgomeryModExp::cleanup() {
    if (ctx) BN_CTX_free(ctx);
    if (N) BN_free(N);
    if (R) BN_free(R);
    if (R2) BN_free(R2);
    if (N_) BN_free(N_);
}

void MontgomeryModExp::computeNPrime() {
    BIGNUM *tmp = BN_new();
    BIGNUM *one = BN_new();

    // 设置1
    BN_one(one);

    // 计算N^(-1) mod R
    BN_mod_inverse(tmp, N, R, ctx);

    // 计算 -N^(-1) mod R
    BN_sub(N_, R, tmp);

    BN_free(tmp);
    BN_free(one);
}

void MontgomeryModExp::REDC(BIGNUM* T, BIGNUM* result) {
    BIGNUM *m = BN_new();
    BIGNUM *t = BN_new();
    BIGNUM *tmp = BN_new();

    BN_copy(tmp, T);
    BN_mask_bits(tmp, k);
    BN_mod_mul(m, tmp, N_, R, ctx);

    // t = (T + mN) / R
    BN_mul(tmp, m, N, ctx);
    BN_add(t, T, tmp);
    BN_rshift(t, t, k);

    if (BN_cmp(t, N) >= 0) {
        BN_sub(result, t, N);
    } else {
        BN_copy(result, t);
    }

    BN_free(m);
    BN_free(t);
    BN_free(tmp);
}

void MontgomeryModExp::mont_mul(const BIGNUM* a, const BIGNUM* b, BIGNUM* result) {
    BIGNUM* T = BN_new();
    BN_mul(T, a, b, ctx);
    REDC(T, result);
    BN_free(T);
}

void MontgomeryModExp::toMontgomeryForm(const BIGNUM* a, BIGNUM* result) {
    BIGNUM *tmp = BN_new();
    BN_mod_mul(tmp, a, R2, N, ctx);
    REDC(tmp, result);
    BN_free(tmp);
}

void MontgomeryModExp::fromMontgomeryForm(const BIGNUM* a, BIGNUM* result) {
    BIGNUM *one = BN_new();
    BN_one(one);
    BIGNUM *tmp = BN_new();
    BN_mul(tmp, a, one, ctx);
    REDC(tmp, result);
    BN_free(one);
    BN_free(tmp);
}

void MontgomeryModExp::mont_exp(const BIGNUM* base, const BIGNUM* exp, BIGNUM* result) {
        // 转换底数到Montgomery域
        BIGNUM *mont_base = BN_new();
        toMontgomeryForm(base, mont_base);

        // 初始化结果为1的Montgomery形式
        BIGNUM *mont_result = BN_new();
        BN_one(mont_result);
        toMontgomeryForm(mont_result, mont_result);

        // 按位进行幂运算
        for (int i = BN_num_bits(exp) - 1; i >= 0; i--)
        {
            // 平方
            mont_mul(mont_result, mont_result, mont_result);

            // 如果当前位为1，乘以底数
            if (BN_is_bit_set(exp, i))
            {
                mont_mul(mont_result, mont_base, mont_result);
            }
        }

        // 转换结果回普通域
        fromMontgomeryForm(mont_result, result);

        BN_free(mont_base);
        BN_free(mont_result);
}