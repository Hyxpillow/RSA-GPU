#include "rsa.h"
#include "utils/obn.h"
#include "utils/config.h"
#include "modexp/modexp_mong_cpu.h"
#include <vector>
#include <openssl/bn.h>

// void do_rsa(const std::vector<BIGNUM*>& input_blocks, 
//             const BIGNUM* exponent, 
//             const BIGNUM* modulus, 
//             std::vector<BIGNUM*>& output_blocks)
// {
//     for (int i = 0; i < input_blocks.size(); i++) {
//         BIGNUM* output_block_i = BN_new();
//         output_blocks.push_back(output_block_i);
//     }
//     for (int i = 0; i < input_blocks.size(); i++) {
//         // BN_CTX *ctx = BN_CTX_new();
//         // BN_mod_exp(output_blocks[i], input_blocks[i], exponent, modulus, ctx);
//         // BN_CTX_free(ctx);
//         MontgomeryModExp montgomery_mod_exp(modulus);
//         montgomery_mod_exp.mont_exp(input_blocks[i], exponent, output_blocks[i]);
//     }
// }

void do_rsa(const std::vector<OURBIGNUM> &input_blocks, 
            std::vector<OURBIGNUM> &output_blocks, 
            const BN_CONFIG &bn_config, const ModType type)
{
    for (int i = 0; i < input_blocks.size(); i++) {
        OURBIGNUM output_block_i;
        output_blocks.push_back(output_block_i);
    }
    for (int i = 0; i < input_blocks.size(); i++) {
        if (type == BASE) {
            // OBN_mod_exp(&output_blocks[i], &input_blocks[i], &bn_config.N, &bn_config.N);
            continue;
        } else if (type == Montgomery_CPU) {
            mont_mod_exp(input_blocks[i], bn_config.E, output_blocks[i], bn_config);
        } else if (type == Montgomery_GPU) {
            continue;
        }
    }
}