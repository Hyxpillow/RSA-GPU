#include "rsa.h"
#include "utils/obn.h"
#include "utils/config.h"
#include "modexp/modexp_cpu_mont.h"
#include "modexp/modexp_cpu.h"
#include <vector>
#include <openssl/bn.h>
#include <string.h>
#include <chrono>

void do_rsa(const std::vector<BIGNUM*> &input_blocks, 
            std::vector<BIGNUM*> &output_blocks, 
            const BN_CONFIG &bn_config, const char* processor)
{
    
    auto start = std::chrono::steady_clock::now();
    for (int i = 0; i < input_blocks.size(); i++) {
        OURBIGNUM *input_block_obn = OBN_new();
        OBN_bn2obn(input_block_obn, input_blocks[i]);
        OURBIGNUM *output_block_obn = OBN_new();
        BIGNUM *output_block = BN_new();
 
        if (strcmp(processor, "cpu") == 0) {
            base_mod_exp(output_block_obn, input_block_obn, bn_config.E, bn_config.N);
        } else if (strcmp(processor, "cpu_mont") == 0) {
            mont_mod_exp(output_block_obn, input_block_obn, bn_config.E, bn_config);
        } else if (strcmp(processor, "gpu_mont") == 0) {
            ;
        }
        OBN_obn2bn(&output_block, output_block_obn);
        output_blocks.push_back(output_block);
    }
    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    printf("Time taken: %.3f s\n", elapsed.count());

}