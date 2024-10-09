#pragma once
#include <utility>
#define BIG_NUM_CAPACITY 1024

struct BigNum {
    size_t length;
    unsigned char data[BIG_NUM_CAPACITY];
    BigNum();
    BigNum(unsigned char x);
    BigNum(const BigNum& x);
};



void multiply(const BigNum& num1,
                const BigNum& num2,
                BigNum& result);
