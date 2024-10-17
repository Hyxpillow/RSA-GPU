#include <iostream>
#include <string.h>
#include "utils/parse_private_key.h"
#include "utils/parse_public_key.h"
#include "utils/pkcs1/pkcs1.h"
#include "utils/parse_key.h"
#include "cpu/big_num.h"
#include "cpu/rsa_cpu.h"
#include <vector>

void print_usage() {
    printf("Usage: program <keyfile> <encrypt|decrypt> <inputfile> <outputfile> <gpu|cpu>\n");
}

int main(int argc, char *argv[]) {
    if (argc != 6) {
        print_usage();
        return 1;
    }
    char *key_file_name = argv[1];
    char *mode = argv[2];
    char *input_file_name = argv[3];
    char *output_file_name = argv[4];
    char *processor = argv[5];

    if (strcmp(mode, "encrypt") != 0 && strcmp(mode, "decrypt") != 0) {
        print_usage();
        return 1;
    }
    if (strcmp(processor, "gpu") != 0 && strcmp(processor, "cpu") != 0) {
        print_usage();
        return 1;
    }

    printf("Key file: %s\n", key_file_name);
    printf("Mode: %s\n", mode);
    printf("Input file: %s\n", input_file_name);
    printf("Output file: %s\n", output_file_name);
    printf("Processor: %s\n", processor);

    BigNum modulus;
    BigNum exponent;
    parse_key(key_file_name, exponent, modulus);

    // @param length: module length
    // @param input_file_name: input file name
    // @param flag: padding flag 0x01 or 0x02
    // @return: a vector of BigNums
    std::vector<BigNum> input_blocks, output_blocks;

    if (strcmp(mode, "encrypt") == 0)
        input_blocks = load_and_pad_file(modulus.length, input_file_name, 0x01);
    if (strcmp(mode, "decrypt") == 0);
        // input_blocks = load_and_not_pad_file()
    
    if (strcmp(processor, "cpu") == 0)
        rsa_cpu(input_blocks, exponent, modulus, output_blocks);
    if (strcmp(processor, "cpu") == 0);
        // rsa_gpu();

    if (strcmp(mode, "encrypt") == 0);
        // save output_blocks to file
    if (strcmp(mode, "decrypt") == 0);
        // save output_blocks to file (remove padding)

    return 0;
}
