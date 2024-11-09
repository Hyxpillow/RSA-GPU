#pragma once

#include <openssl/bn.h>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

// 普通模幂实现
class ModExp {
private:
    BN_CTX* ctx;

public:
    ModExp();
    ~ModExp();
    void mod_exp(const BIGNUM* base, const BIGNUM* exp, const BIGNUM* n, BIGNUM* result);
};

// Montgomery模幂实现
class MontgomeryModExp {
private:
    BN_CTX* ctx;
    BIGNUM* N;    // 模数
    BIGNUM* R;    // Montgomery基数 R = 2^k
    BIGNUM* R2;   // R^2 mod N
    BIGNUM* N_;   // N' = -N^(-1) mod R
    int k;        // 模数的位长

    void computeNPrime();
    void REDC(BIGNUM* T, BIGNUM* result);
    void cleanup();

public:
    explicit MontgomeryModExp(const BIGNUM* modulus);
    ~MontgomeryModExp();
    
    void toMontgomeryForm(const BIGNUM *a, BIGNUM *result);
    void fromMontgomeryForm(const BIGNUM *a, BIGNUM *result);
    void mont_mul(const BIGNUM* a, const BIGNUM* b, BIGNUM* result);
    void mont_exp(const BIGNUM* base, const BIGNUM* exp, BIGNUM* result);
};

// 测试类
class ModExpTester {
private:
    BN_CTX* ctx;
    
    struct TestCase {
        std::string base;
        std::string exp;
        std::string mod;
        std::string expected;
    };

    std::vector<TestCase> test_cases;
    
    void run_single_test(const TestCase& test);
    void run_random_tests(int count);

public:
    ModExpTester();
    ~ModExpTester();
    void run_all_tests();
};