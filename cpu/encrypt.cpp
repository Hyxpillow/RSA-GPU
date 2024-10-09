#include "big_num.h"
#include "encrypt.h"

BigNum encrypt(const BigNum& msg, 
            const BigNum& exp, 
            const BigNum& mod) 
{
    BigNum res(1);
    BigNum xxx(msg);
    for (size_t byte_idx = 0; byte_idx < msg.length; byte_idx++) 
    {
        BigNum buffer;
        for (unsigned char bit_mask = 1; bit_mask != 0; bit_mask <<= 1) 
        {
            if (exp.data[byte_idx] & bit_mask) 
            {   
                multiply(res, xxx, buffer);
                res = buffer;
            }
            multiply(xxx, xxx, buffer);
            xxx = buffer;
        }
    }
    return res;
}