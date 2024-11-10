#include "modexp.h"
#include <iostream>
#include <cassert>

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

// ModExpTester 实现
ModExpTester::ModExpTester() : ctx(BN_CTX_new()) {
    if (!ctx) throw std::runtime_error("Failed to create BN_CTX");
    
    // 初始化测试用例
    test_cases = {
        {"2", "3", "5", "3"},
        {"123", "17", "65537", ""},
        {"1", "1", "3", "1"},
        {"0", "1", "7", "0"},
        {"2", "0", "11", "1"},
        {"65537", "3", "294877", ""},
        {"2", "16", "65537", ""},
        {"123456789", "17", "999999937", ""},
        {"2", "31", "65537", ""},
        {"5", "11", "14", ""},
        {"13", "3", "17", ""}
    };
}

ModExpTester::~ModExpTester() {
    if (ctx) BN_CTX_free(ctx);
}

void ModExpTester::run_single_test(const TestCase& test) {
    BIGNUM *base = NULL, *exp = NULL, *mod = NULL;
    BIGNUM *result_normal = BN_new();
    BIGNUM *result_mont = BN_new();
    BIGNUM *result_openssl = BN_new();

    try {
        base = BN_new();
        exp = BN_new();
        mod = BN_new();
        BN_dec2bn(&base, test.base.c_str());
        BN_dec2bn(&exp, test.exp.c_str());
        BN_dec2bn(&mod, test.mod.c_str());

        std::cout << "Base: " << test.base << std::endl;
        std::cout << "Exp:  " << test.exp << std::endl;
        std::cout << "Mod:  " << test.mod << std::endl;

        // 测试普通模幂
        ModExp normal_exp;
        normal_exp.mod_exp(base, exp, mod, result_normal);

        // 测试Montgomery模幂
        MontgomeryModExp mont_exp(mod);
        mont_exp.mont_exp(base, exp, result_mont);

        // OpenSSL参考实现
        BN_mod_exp(result_openssl, base, exp, mod, ctx);

        // 输出结果
        char *str_normal = BN_bn2dec(result_normal);
        char *str_mont = BN_bn2dec(result_mont);
        char *str_openssl = BN_bn2dec(result_openssl);

        std::cout << "Normal Result:     " << str_normal << std::endl;
        std::cout << "Montgomery Result: " << str_mont << std::endl;
        std::cout << "OpenSSL Result:    " << str_openssl << std::endl;

        // 验证结果
        bool normal_match = (BN_cmp(result_normal, result_openssl) == 0);
        bool mont_match = (BN_cmp(result_mont, result_openssl) == 0);

        std::cout << "Normal Implementation: " 
                  << (normal_match ? "PASSED" : "FAILED") << std::endl;
        std::cout << "Montgomery Implementation: " 
                  << (mont_match ? "PASSED" : "FAILED") << std::endl;

        OPENSSL_free(str_normal);
        OPENSSL_free(str_mont);
        OPENSSL_free(str_openssl);

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    BN_free(base);
    BN_free(exp);
    BN_free(mod);
    BN_free(result_normal);
    BN_free(result_mont);
    BN_free(result_openssl);
}

void ModExpTester::run_random_tests(int count) {
    std::cout << "\nRunning Random Tests...\n" << std::endl;
    
    for (int i = 0; i < count; i++) {
        BIGNUM *base = BN_new();
        BIGNUM *exp = BN_new();
        BIGNUM *mod = BN_new();
        
        BN_rand(base, 256, -1, 0);
        BN_rand(exp, 64, -1, 0);
        BN_rand(mod, 256, -1, 1);

        char *base_str = BN_bn2dec(base);
        char *exp_str = BN_bn2dec(exp);
        char *mod_str = BN_bn2dec(mod);

        TestCase random_test{base_str, exp_str, mod_str, ""};

        std::cout << "Random Test " << (i + 1) << ":" << std::endl;
        run_single_test(random_test);
        std::cout << "------------------------" << std::endl;

        OPENSSL_free(base_str);
        OPENSSL_free(exp_str);
        OPENSSL_free(mod_str);
        BN_free(base);
        BN_free(exp);
        BN_free(mod);
    }
}

void ModExpTester::run_all_tests() {
    std::cout << "\nRunning ModExp Tests...\n" << std::endl;
    int test_num = 1;
    
    for (const auto& test : test_cases) {
        std::cout << "Test Case " << test_num++ << ":" << std::endl;
        run_single_test(test);
        std::cout << "------------------------" << std::endl;
    }
    
    run_random_tests(10);
}

// 主函数
int main() {
    try {
        ModExpTester tester;
        tester.run_all_tests();
        std::cout << "\nAll tests completed successfully!" << std::endl;
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Test suite failed: " << e.what() << std::endl;
        return 1;
    }
}