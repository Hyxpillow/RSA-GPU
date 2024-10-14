#pragma once
#include "big_num.h"

void encrypt(const BigNum& msg, 
            const BigNum& exp, 
            const BigNum& mod,
            BigNum& res);