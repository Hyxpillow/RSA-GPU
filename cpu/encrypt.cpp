#include "big_num.h"
#include "encrypt.h"
#include <string.h>
#include <iostream>

void encrypt(const BigNum& msg, 
            const BigNum& exp, 
            const BigNum& mod,
            BigNum& res) 
{
    memset(res.data, 0, BIG_NUM_CAPACITY);
    res.data[0] = 1;
    res.length = 1;
    BigNum xxx(msg);
    for (size_t byte_idx = 0; byte_idx < exp.length; byte_idx++) 
    {
        BigNum buffer;
        for (unsigned char bit_mask = 1; bit_mask != 0; bit_mask <<= 1) 
        {
            if (exp.data[byte_idx] & bit_mask) 
            {   
                std::cout << "res1: "  << res.toDecimal() << std::endl;
                multiply(res, xxx, buffer);
                std::cout << "res2: "  << buffer.toDecimal() << std::endl;
                modulo(buffer, mod, res);
                std::cout << "res3: "  << res.toDecimal() << std::endl;
            }
            std::cout << "before buffer: " << buffer.toDecimal() << std::endl;
            multiply(xxx, xxx, buffer);
            std::cout << "after buffer: " << buffer.toDecimal() << std::endl;

            std::cout << "before xxx: " << xxx.toDecimal() << std::endl;
            modulo(buffer, mod, xxx);
            std::cout << "after xxx: " << xxx.toDecimal() << std::endl;
        }
    }
}