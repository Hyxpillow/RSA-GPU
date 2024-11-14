#include "config.h"
#include "obn.h"
#include <iostream>
#include <cassert>
BN_CONFIG::BN_CONFIG(const BIGNUM* modulus, const BIGNUM* e) {
        ctx = BN_CTX_new();
        if (ctx == nullptr)
        {
            throw std::runtime_error("Failed to create BN_CTX");
        }

        // 分配内存
        _N  = BN_new();
        _R  = BN_new();
        _R2 = BN_new();
        _N_ = BN_new();
        _E  = BN_new();

        if (!_N || !_R || !_R2 || !_N_)
        {
            throw std::runtime_error("Failed to allocate BIGNUM");
        }

        // 复制模数
        BN_copy(_N, modulus);
        BN_copy(_E, e);

        // 计算k：N的位长
        k = BN_num_bits(_N);
        k = ((k + 31) / 32) * 32;

        // 计算R = 2^k
        BN_set_bit(_R, k);

        // 计算R2 mod N
        BN_mod_mul(_R2, _R, _R, _N, ctx);

        // 计算N' = -N^(-1) mod R
        computeNPrime();

        N  = OBN_new();
        R  = OBN_new();
        R2 = OBN_new();
        N_ = OBN_new();
        E  = OBN_new();

        // 将BIGNUM转换为OURBIGNUM
        OBN_bn2obn(N, _N);
        OBN_bn2obn(R, _R);
        OBN_bn2obn(R2, _R2);
        OBN_bn2obn(N_, _N_);
        OBN_bn2obn(E, _E);
}

BN_CONFIG::~BN_CONFIG() {
    cleanup();
}

void BN_CONFIG::cleanup() {
    if (ctx) BN_CTX_free(ctx);
    if (_N) BN_free(_N);
    if (_R) BN_free(_R);
    if (_R2) BN_free(_R2);
    if (_N_) BN_free(_N_);
    if (_E) BN_free(_E);

    if (N) OBN_free(N);
    if (R) OBN_free(R);
    if (R2) OBN_free(R2);
    if (N_) OBN_free(N_);
    if (E) OBN_free(E);
}

void BN_CONFIG::computeNPrime() {
    BIGNUM *tmp = BN_new();


    // 计算N^(-1) mod R
    BN_mod_inverse(tmp, _N, _R, ctx);

    // 计算 -N^(-1) mod R
    BN_sub(_N_, _R, tmp);

    BN_free(tmp);
}