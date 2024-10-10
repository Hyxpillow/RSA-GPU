#include <gtest/gtest.h>
#include <iostream>
#include <string>
#include "big_num.h"

uint64_t BigNumToDecimal(const BigNum &num) {
    uint64_t result = 0;

    std::cout << "num.length: " << num.length << std::endl;
    for (size_t i = 0; i < num.length; ++i) {
        result += static_cast<uint64_t>(num.data[i]) * std::pow(256, i);  // 每一位乘以 256 的幂次
    }
    return result;
}

uint64_t referenceMod(uint64_t num1, uint64_t num2) {
    return num1 % num2;
}

TEST(BigNumTest, SubtractionTest) {
    BigNum num1("1000");
    BigNum num2("123");
    BigNum result;

    subtract(num1, num2, result);

    BigNum expected("877");

    uint64_t actual = BigNumToDecimal(result);
    uint64_t expected_val = BigNumToDecimal(expected);

    EXPECT_EQ(actual, expected_val);
}

TEST(BigNumTest, ShiftTest) {
    BigNum num("1");
    shiftRight(num);

    BigNum expected("256");

    uint64_t actual = BigNumToDecimal(num);
    uint64_t expected_val = BigNumToDecimal(expected);

    EXPECT_EQ(actual, expected_val);
}


TEST(BigNumTest, ModulusTestSmallNumbers) {
    BigNum num1("120");
    BigNum num2("11");
    BigNum result;

    mod(num1, num2, result);

    uint64_t actual = BigNumToDecimal(result);

    uint64_t expected = BigNumToDecimal(num1) % BigNumToDecimal(num2);

    EXPECT_EQ(actual, expected);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
