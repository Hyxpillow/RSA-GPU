#include <gtest/gtest.h>
#include <iostream>
#include <string>
#include "big_num.h"
#include "encrypt.h"


void printBigNumInBinary(const BigNum &num) {
    // 遍历 num.data 的每个字节，逐字节打印二进制值
    for (size_t i = 0; i < num.length; ++i) {
        // 使用 bitset 来将每个字节转换为二进制字符串
        std::bitset<8> byte(num.data[i]);  // 8位表示一个字节
        std::cout << byte << " ";
    }
    std::cout << std::endl;  // 打印结束，换行
}

uint64_t referenceMod(uint64_t num1, uint64_t num2) {
    return num1 % num2;
}

TEST(BigNumTest, SubtractionTest) {

    BigNum num1("10000");

    BigNum num2("123");
    BigNum result;

    subtract(num1, num2, result);


    BigNum expected("9877");


    uint64_t actual = result.toDecimal();
    uint64_t expected_val = expected.toDecimal();

    EXPECT_EQ(actual, expected_val);
}

TEST(BigNumTest, ShiftTest) {
    BigNum num("1");

    shiftLeft(num);


    BigNum expected("256");

    uint64_t actual = num.toDecimal();
    uint64_t expected_val = expected.toDecimal();

    EXPECT_EQ(actual, expected_val);
}


TEST(BigNumTest, ModulusTestSmallNumbers) {
    BigNum num1("271");
    BigNum num2("323");
    BigNum result;

    modulo(num1, num2, result);
    uint64_t actual = result.toDecimal();
    uint64_t expected = num1.toDecimal() % num2.toDecimal();

    EXPECT_EQ(actual, expected);
}

TEST(BigNumTest, MultiplySmallNumber) {
    BigNum num1("123");
    BigNum num2("123");
    BigNum result;

    multiply(num1, num2, result);

    uint64_t actual = result.toDecimal();
    uint64_t expected = 15129;
    EXPECT_EQ(actual, expected);
}

TEST(EncryptTest, DISABLED_EncryptSmallNumber) {
    BigNum msg("123");
    BigNum exp("2");
    BigNum mod("323");
    BigNum result;

    encrypt(msg, exp, mod, result);

    uint64_t actual = result.toDecimal();
    uint64_t expected = 225;
    EXPECT_EQ(actual, expected);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
