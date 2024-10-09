#include "big_num.h"
#include <string.h>

BigNum::BigNum()
{
    memset(this->data, 0, BIG_NUM_CAPACITY);
}
BigNum::BigNum(unsigned char x)
{
    memset(this->data, 0, BIG_NUM_CAPACITY);
    this->data[0] = x;
}
BigNum::BigNum(const BigNum &x)
{
    memcpy(this->data, x.data, BIG_NUM_CAPACITY);
}

void multiply(const BigNum &num1,
              const BigNum &num2,
              BigNum &result)
{
    memset(&result, 0, BIG_NUM_CAPACITY);
    for (size_t i = 0; i < num1.length; ++i)
    {
        unsigned int carry = 0;
        for (size_t j = 0; j < num2.length; ++j)
        {
            unsigned int product =
                result.data[i + j] +
                num1.data[i] * num2.data[j] +
                carry;

            result.data[i + j] = product & 0xFF;
            carry = product >> 8;
        }

        result.data[i + num2.length] += carry;
    }
}