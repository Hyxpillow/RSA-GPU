#include "rsa_cpu.h"
#include <vector>

void rsa_cpu(const std::vector<BigNum>& input_blocks, 
            const BigNum& exponent, 
            const BigNum& modulus, 
            std::vector<BigNum>& output_blocks)
{
    for (int i = 0; i < input_blocks.size(); i++) {
        BigNum output_block_i;
        exponentiate_modulo(
            input_blocks[i],
            exponent,
            modulus,
            output_block_i
        );
        output_blocks.push_back(output_block_i);
    }
}