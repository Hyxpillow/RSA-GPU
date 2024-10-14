#include "big_num.h"
#include "encrypt.h"
#include <string.h>
#include <iostream>

void encrypt(const BigNum& msg, 
            const BigNum& exp, 
            const BigNum& mod,
            BigNum& res) 
{
    if (msg.length == 0)
    {
        res = 0;
        return;
    }
    res = 1;
    BigNum xxx(msg);
    for (size_t byte_idx = 0; byte_idx < exp.length; byte_idx++) 
    {
        BigNum buffer;
        for (unsigned char bit_mask = 1; bit_mask != 0; bit_mask <<= 1) 
        {
            if (exp.data[byte_idx] & bit_mask) 
            {   
                multiply(res, xxx, buffer);
                modulo(buffer, mod, res);
            }
            multiply(xxx, xxx, buffer);
            modulo(buffer, mod, xxx);
        }
    }
}