#pragma once

#include <openssl/bn.h>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>
#include "obn.h"
class BN_CONFIG {
public:
    BN_CTX* ctx;
    BIGNUM* _N;    //
    BIGNUM* _R;    // Montgomer R = 2^k
    BIGNUM* _R2;   // R^2 mod N
    BIGNUM* _N_;   // N' = -N^(-1) mod R
    BIGNUM* _E;     //
           
    void computeNPrime();
    void cleanup();
public:
    OURBIGNUM* N;
    OURBIGNUM* R;
    OURBIGNUM* R2;
    OURBIGNUM* N_;
    OURBIGNUM* E;
    int k; 

    explicit BN_CONFIG(const BIGNUM* modulus, const BIGNUM* e);
    ~BN_CONFIG();
};
