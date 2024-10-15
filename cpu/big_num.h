#pragma once
#include <utility>
#define BIG_NUM_CAPACITY 1024

struct BigNum {
    size_t length;
    unsigned char data[BIG_NUM_CAPACITY];
    BigNum();
    BigNum(unsigned char);
    BigNum(const BigNum&);
    BigNum(const std::string&);

    void convertFromDecimalString(const std::string &);
    uint64_t toDecimal() const;

    std::string toString();

    BigNum& operator=(const BigNum &);
    BigNum& operator=(unsigned char);
};



void multiply(const BigNum& num1,
                const BigNum& num2,
                BigNum& result);

void modulo(const BigNum& Dividend,
            const BigNum& Divisor,
            BigNum& remainder);

void subtract(const BigNum& a, const BigNum& b, BigNum& result);

void shiftLeft(BigNum &num);
int compare(const BigNum& a, const BigNum& b);


void exponentiate_modulo(const BigNum& msg, 
            const BigNum& exp, 
            const BigNum& mod,
            BigNum& res);