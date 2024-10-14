#pragma once
#include <utility>
#define BIG_NUM_CAPACITY 1024

struct BigNum {
    size_t length;
    unsigned char data[BIG_NUM_CAPACITY];
    BigNum();
    BigNum(unsigned char x);
    BigNum(const BigNum& x);
    BigNum(const std::string &numStr);

    void convertFromDecimalString(const std::string &numStr);

    BigNum& operator=(const BigNum &other);
};



void multiply(const BigNum& num1,
                const BigNum& num2,
                BigNum& result);

void mod(const BigNum& Dividend,
            const BigNum& Divisor,
            BigNum& remainder);

void subtract(const BigNum& a, const BigNum& b, BigNum& result);

void shiftLeft(BigNum &num);
int compare(const BigNum& a, const BigNum& b);