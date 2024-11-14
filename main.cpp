#include <iostream>
#include <string.h>
#include <vector>
#include <openssl/bn.h>
#include <openssl/rsa.h>
#include "utils/read_rsa_key.h"
#include "utils/padding.h"
#include "utils/config.h"
#include "utils/obn.h"
#include "rsa.h"

int main(int argc, char *argv[])
{
    if (argc < 6)
    {
        printf("Usage: program <encrypt|decrypt> <keyfile> <inputfile> <outputfile> <gpu|cpu>\n");
        return 1;
    }
    char *mode = argv[1];
    char *key_file_name = argv[2];
    char *input_file_name = argv[3];
    char *output_file_name = argv[4];
    char *processor = argv[5];

    if (strcmp(mode, "encrypt") != 0 &&
            strcmp(mode, "decrypt") != 0 ||
        strcmp(processor, "gpu") != 0 &&
            strcmp(processor, "cpu") != 0)
    {
        printf("Usage: program <encrypt|decrypt> <keyfile> <inputfile> <outputfile> <gpu|cpu>\n");
        return 1;
    }

    RSA *rsa = read_rsa_key(key_file_name);
    const BIGNUM *modulus = RSA_get0_n(rsa);
    const BIGNUM *e = RSA_get0_e(rsa);
    const BIGNUM *d = RSA_get0_d(rsa);
    const BIGNUM *exponent = (d != NULL) ? d : e;

    const BN_CONFIG bn_config(modulus, exponent);
    std::vector<BIGNUM*> input_blocks, output_blocks;
    if (strcmp(mode, "encrypt") == 0)
    {
        load_and_pad_file(BN_num_bytes(modulus), input_file_name, 0x01, input_blocks);
        do_rsa(input_blocks, output_blocks, bn_config, Montgomery_CPU);
        save_not_pad_file(BN_num_bytes(modulus), output_file_name, output_blocks);
    }
    else if (strcmp(mode, "decrypt") == 0)
    {
        load_and_not_pad_file(BN_num_bytes(modulus), input_file_name, input_blocks);
        do_rsa(input_blocks, output_blocks, bn_config, Montgomery_CPU);
        save_pad_file(BN_num_bytes(modulus), output_file_name, 0x01, output_blocks);
    }
    RSA_free(rsa);
    return 0;
}
