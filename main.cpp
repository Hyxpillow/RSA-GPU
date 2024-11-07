#include <iostream>
#include <string.h>
#include "utils/parse_private_key.h"
#include "utils/parse_public_key.h"
#include "utils/pkcs1/pkcs1.h"
#include "utils/parse_key.h"
#include "cpu/rsa_cpu.h"
#include <vector>
#include <openssl/bn.h>

void print_usage() {
    printf("Usage: program <encrypt|decrypt|factorize>\n");
}

void print_usage_enc() {
    printf("Usage: program encrypt <keyfile> <inputfile> <outputfile> <gpu|cpu>\n");
}

void print_usage_dec() {
    printf("Usage: program decrypt <keyfile> <inputfile> <outputfile> <gpu|cpu>\n");
}

void print_usage_fac() {
    printf("Usage: program factorize <keyfile> <gpu|cpu>\n"); 
}

void encrypt(char *mode,
    char *key_file_name,
    char *input_file_name,
    char *output_file_name,
    char *processor) 
{
    BIGNUM *modulus = BN_new();
    BIGNUM *exponent = BN_new();
    parse_key(key_file_name, exponent, modulus);


    std::vector<BIGNUM*> input_blocks, output_blocks;

    input_blocks = load_and_pad_file(BN_num_bytes(modulus), input_file_name, 0x01);
    
    if (strcmp(processor, "cpu") == 0)
        rsa_cpu(input_blocks, exponent, modulus, output_blocks);
    // if (strcmp(processor, "cpu") == 0);
    //     1; // rsa_gpu();

    save_not_pad_file(BN_num_bytes(modulus), output_file_name, output_blocks);
}

void decrypt(char *mode,
    char *key_file_name,
    char *input_file_name,
    char *output_file_name,
    char *processor) 
{
    BIGNUM *modulus = BN_new();
    BIGNUM *exponent = BN_new();
    parse_key(key_file_name, exponent, modulus);


    std::vector<BIGNUM*> input_blocks, output_blocks;
    
    input_blocks = load_and_not_pad_file(BN_num_bytes(modulus), input_file_name);
    
    if (strcmp(processor, "cpu") == 0)
        rsa_cpu(input_blocks, exponent, modulus, output_blocks);
    // if (strcmp(processor, "cpu") == 0);
    //     1; // rsa_gpu();

    save_pad_file(BN_num_bytes(modulus), output_file_name, 0x01, output_blocks);
}

void factorize() {

}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        print_usage();
        return 1;
    }
    char *mode = argv[1];

    char *key_file_name;
    char *input_file_name;
    char *output_file_name;
    char *processor;

    if (strcmp(mode, "factorize") == 0) {
        if (argc != 4) {
            print_usage_fac();
            return 1;
        }
        key_file_name = argv[2];
        processor = argv[3];
        if (strcmp(processor, "gpu") != 0 && strcmp(processor, "cpu") != 0) {
            print_usage();
            return 1;
        }
        factorize();
    } else if (strcmp(mode, "encrypt") == 0) {
        if (argc != 6) {
            print_usage_enc();
            return 1;
        }
        key_file_name = argv[2];
        input_file_name = argv[3];
        output_file_name = argv[4];
        processor = argv[5];
        if (strcmp(processor, "gpu") != 0 && strcmp(processor, "cpu") != 0) {
            print_usage();
            return 1;
        }
        encrypt(mode, key_file_name, input_file_name, output_file_name, processor);
    } else if (strcmp(mode, "decrypt") == 0) {
        if (argc != 6) {
            print_usage_dec();
            return 1;
        }
        key_file_name = argv[2];
        input_file_name = argv[3];
        output_file_name = argv[4];
        processor = argv[5];
        decrypt(mode, key_file_name, input_file_name, output_file_name, processor);
        if (strcmp(processor, "gpu") != 0 && strcmp(processor, "cpu") != 0) {
            print_usage();
            return 1;
        }
    } else {
        print_usage();
        return 1;
    }

    return 0;
}
