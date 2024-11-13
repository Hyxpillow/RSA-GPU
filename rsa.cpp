#include "rsa.h"
#include "utils/obn.h"
#include "utils/config.h"
#include "modexp/modexp_mong_cpu.h"
#include "modexp/modexp_base.h"
#include <vector>
#include <openssl/bn.h>

void do_rsa(const std::vector<BIGNUM*> &input_blocks, 
            std::vector<BIGNUM*> &output_blocks, 
            const BN_CONFIG &bn_config, const ModType type)
{
    
    for (int i = 0; i < input_blocks.size(); i++) {
        OURBIGNUM *input_block_obn = OBN_new();
        OBN_bn2obn(input_block_obn, input_blocks[i]);
        OURBIGNUM *output_block_obn = OBN_new();
        BIGNUM *output_block = BN_new();

        if (type == BASE) {
            base_mod_exp(input_block_obn, bn_config.E, output_block_obn, bn_config.N);
        } else if (type == Montgomery_CPU) {
            mont_mod_exp(input_block_obn, bn_config.E, output_block_obn, bn_config);
        } else if (type == Montgomery_GPU) {
            ;
        }
        output_blocks.push_back(output_block);

    }
}