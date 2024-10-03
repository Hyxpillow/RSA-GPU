
#pragma once
#include "data_buffer.h"

class Rsa {
public:
    virtual DataBuffer encrypt(const DataBuffer& input, 
                                const DataBuffer& exponent, 
                                const DataBuffer& modulus) = 0;
    
    virtual std::pair<DataBuffer, DataBuffer> factorize(const DataBuffer& input) = 0;
};