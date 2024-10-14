#include <gtest/gtest.h>
#include <iostream>
#include <string>
#include "big_num.h"


void printBigNumInBinary(const BigNum &num) {
    // 遍历 num.data 的每个字节，逐字节打印二进制值
    for (size_t i = 0; i < num.length; ++i) {
        // 使用 bitset 来将每个字节转换为二进制字符串
        std::bitset<8> byte(num.data[i]);  // 8位表示一个字节
        std::cout << byte << " ";
    }
    std::cout << std::endl;  // 打印结束，换行
}

uint64_t BigNumToDecimal(const BigNum &num) {
    uint64_t result = 0;
    uint64_t base = 1;
    for (size_t i = 0; i < num.length; ++i) {
        result += static_cast<uint64_t>(num.data[i]) * base;
        base <<= 8;
    }
    return result;
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


    uint64_t actual = BigNumToDecimal(result);
    uint64_t expected_val = BigNumToDecimal(expected);

    EXPECT_EQ(actual, expected_val);
}

TEST(BigNumTest, ShiftTest) {
    BigNum num("1");

    shiftLeft(num);


    BigNum expected("256");

    uint64_t actual = BigNumToDecimal(num);
    uint64_t expected_val = BigNumToDecimal(expected);

    EXPECT_EQ(actual, expected_val);
}


TEST(BigNumTest, ModulusTestSmallNumbers) {
    BigNum num1("12000");
    BigNum num2("1111");
    BigNum result;

    mod(num1, num2, result);
    printBigNumInBinary(result);
    uint64_t actual = BigNumToDecimal(result);

    uint64_t expected = BigNumToDecimal(num1) % BigNumToDecimal(num2);
    printBigNumInBinary(BigNum(std::to_string(expected)));

    EXPECT_EQ(actual, expected);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
