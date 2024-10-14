#include "big_num.h"
#include <iostream>
#include <string.h>
#include <string>
#include <vector>

BigNum::BigNum()
{
    memset(this->data, 0, BIG_NUM_CAPACITY);
    this->length = 0;
}
BigNum::BigNum(unsigned char x)
{
    memset(this->data, 0, BIG_NUM_CAPACITY);
    this->data[0] = x;
    this->length = 1;
}

BigNum::BigNum(const BigNum &x)
{
    memcpy(this->data, x.data, BIG_NUM_CAPACITY);
    this->length = x.length;
}

BigNum& BigNum::operator=(const BigNum &other) {
    if (this != &other) {  // 避免自我赋值
        memcpy(this->data, other.data, BIG_NUM_CAPACITY);  // 深拷贝 data 数组
        this->length = other.length;  // 复制 length
    }
    return *this;
}

BigNum::BigNum(const std::string &numStr) {
    memset(this->data, 0, BIG_NUM_CAPACITY);
    this->length = 0;

    size_t start = 0;
    while (start < numStr.size() && numStr[start] == '0') {
        start++;
    }

    if (start == numStr.size()) {
        this->data[0] = 0;
        this->length = 1;
    } else {
        std::string numPart = numStr.substr(start);
        convertFromDecimalString(numPart);
    }
}

void multiply(const BigNum &num1,
              const BigNum &num2,
              BigNum &result)
{
    memset(result.data, 0, BIG_NUM_CAPACITY);
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
    result.length = BIG_NUM_CAPACITY;
    while (result.data[result.length - 1] == 0)
    {
        result.length--;
    }
}

void modulo(const BigNum &num1, const BigNum &mod, BigNum &result) {
    BigNum currentRemainder;
    memset(&currentRemainder, 0, sizeof(BigNum));
    for (int i = num1.length - 1; i >= 0; --i) {
        shiftLeft(currentRemainder);
        currentRemainder.data[0] = num1.data[i];
        if (currentRemainder.length == 0 && currentRemainder.data[0] != 0) {
            currentRemainder.length = 1;
        } else if (currentRemainder.length > 0 || currentRemainder.data[0] != 0) {
            currentRemainder.length++;
        }
        
        while (compare(currentRemainder, mod) >= 0) {

            BigNum tempRemainder;
            subtract(currentRemainder, mod, tempRemainder);
            currentRemainder = tempRemainder;
        }
    }
    result = currentRemainder;
    while (result.length > 1 && result.data[result.length - 1] == 0) {
        result.length--;
    }

}


void subtract(const BigNum &num1, const BigNum &num2, BigNum &result) {
    memset(result.data, 0, BIG_NUM_CAPACITY);
    result.length = 0;

    unsigned char borrow = 0;

    for (size_t i = 0; i < num1.length; ++i) {
        int current_diff = static_cast<int>(num1.data[i]) - (i < num2.length ? num2.data[i] : 0) - borrow;

        if (current_diff < 0) {
            current_diff += 0x100;
            borrow = 1;
        } else {
            borrow = 0;
        }
        result.data[i] = static_cast<unsigned char>(current_diff);
    }

    result.length = num1.length;
    while (result.length > 1 && result.data[result.length - 1] == 0) {
        result.length--;
    }
}


int compare(const BigNum& a, const BigNum& b) {
    if (a.length != b.length) {
        return a.length > b.length ? 1 : -1;
    }
    for (size_t i = a.length - 1; i >= 0; --i) {
        if (a.data[i] != b.data[i]) {
            return a.data[i] > b.data[i] ? 1 : -1;
        }
    }
    return 0;
}


void shiftLeft(BigNum &num) {
    if (num.length == 0) {
        return;
    }
    for (size_t i = num.length; i > 0; --i) {
        num.data[i] = num.data[i - 1];
    }
    num.data[0] = 0;
    if (num.length < BIG_NUM_CAPACITY) {
        num.length++;
    }
}

void BigNum::convertFromDecimalString(const std::string &numStr) {
    std::vector<unsigned char> decimalDigits;
    for (char ch : numStr) {
        decimalDigits.push_back(ch - '0');
    }

    while (!decimalDigits.empty()) {
        unsigned int remainder = 0;
        std::vector<unsigned char> newDecimalDigits;

        for (size_t i = 0; i < decimalDigits.size(); ++i) {
            unsigned int current = remainder * 10 + decimalDigits[i];
            newDecimalDigits.push_back(current / 256);
            remainder = current % 256;
        }

        this->data[this->length++] = static_cast<unsigned char>(remainder);

        size_t leadingZeros = 0;
        while (leadingZeros < newDecimalDigits.size() && newDecimalDigits[leadingZeros] == 0) {
            leadingZeros++;
        }

        // 更新 decimalDigits 为新的商部分
        decimalDigits = std::vector<unsigned char>(newDecimalDigits.begin() + leadingZeros, newDecimalDigits.end());
    }
}


uint64_t BigNum::toDecimal() {
    uint64_t result = 0;
    uint64_t base = 1;
    for (size_t i = 0; i < this->length; ++i) {
        result += static_cast<uint64_t>(this->data[i]) * base;
        base <<= 8;
    }
    return result;
}