#include "rsa.h"
#include "utils/obn.h"
#include "modexp/modexp_cpu.h"
#include "modexp/modexp_gpu.h"
#include <vector>
#include <openssl/bn.h>
#include <string.h>
#include <chrono>

void do_rsa(const std::vector<BIGNUM*> &input_blocks, 
            std::vector<BIGNUM*> &output_blocks, 
            const BIGNUM* exponent,
            const BIGNUM* modulus,
            const char* processor)
{
    
    OBN_MUL_GPU_CTX *ctx = OBN_MUL_GPU_CTX_new();

    OURBIGNUM *exponent_obn = OBN_new();
    OBN_bn2obn(exponent_obn, exponent);
    OURBIGNUM *modulus_obn = OBN_new();
    OBN_bn2obn(modulus_obn, modulus);

    auto start = std::chrono::steady_clock::now();
    for (int i = 0; i < input_blocks.size(); i++) {
        OURBIGNUM *input_block_obn = OBN_new();
        OBN_bn2obn(input_block_obn, input_blocks[i]);
        OURBIGNUM *output_block_obn = OBN_new();
        BIGNUM *output_block = BN_new();
 
        if (strcmp(processor, "cpu") == 0) {
            mod_exp_cpu(output_block_obn, input_block_obn, exponent_obn, modulus_obn);
        } else if (strcmp(processor, "gpu") == 0) {
            mod_exp_gpu(output_block_obn, input_block_obn, exponent_obn, modulus_obn, ctx);
        }
        OBN_obn2bn(&output_block, output_block_obn);
        output_blocks.push_back(output_block);
    }
    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    printf("Time taken: %.3f s\n", elapsed.count());

    OBN_MUL_GPU_CTX_free(ctx);
}