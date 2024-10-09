#pragma once
#include "big_num.h"

BigNum encrypt(const BigNum& msg_const, 
            const BigNum& exp, 
            const BigNum& mod);