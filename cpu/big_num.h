#pragma once
#include <utility>

class big_num 
{
public:
    big_num operator*(const big_num& other) const;
    big_num operator%(const big_num& other) const;
    big_num operator!=(const big_num& other) const;
    big_num operator>>(const big_num& other) const;
    unsigned char* data;
    size_t length;
};