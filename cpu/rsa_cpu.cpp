#include "rsa_cpu.h"
#include <vector>
#include <openssl/bn.h>

void rsa_cpu(const std::vector<BIGNUM*>& input_blocks, 
            const BIGNUM* exponent, 
            const BIGNUM* modulus, 
            std::vector<BIGNUM*>& output_blocks)
{
    for (int i = 0; i < input_blocks.size(); i++) {
        BIGNUM* output_block_i = BN_new();
        output_blocks.push_back(output_block_i);
    }
    for (int i = 0; i < input_blocks.size(); i++) {
        BN_CTX *ctx = BN_CTX_new();
        BN_mod_exp(output_blocks[i], input_blocks[i], exponent, modulus, ctx);
        BN_CTX_free(ctx);
    }
}