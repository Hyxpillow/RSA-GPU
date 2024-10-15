#include <iostream>
#include <string.h>
#include "utils/utils.h"

void print_usage() {
    printf("Usage: program <keyfile> <inputfile> <outputfile> <encrypt|decrypt> <gpu|cpu>\n");
}

int main(int argc, char *argv[]) {
    if (argc != 6) {
        print_usage();
        return 1;
    }
    char *key_file_name = argv[1];
    char *input_file_name = argv[2];
    char *output_file_name = argv[3];
    char *mode = argv[4];
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
    printf("Input file: %s\n", input_file_name);
    printf("Output file: %s\n", output_file_name);
    printf("Mode: %s\n", mode);
    printf("Processor: %s\n", processor);

    unsigned char* key_buffer = read_file(key_file_name);
    unsigned char* input_buffer = read_file(input_file_name);
    unsigned char* output_buffer;

    unsigned char* modulus;
    unsigned char* exponent;

    if (strcmp(mode, "encrypt") == 0)
        parse_public_key(key_buffer, exponent, modulus);
    if (strcmp(mode, "decrypt") == 0)
        parse_private_key(key_buffer, exponent, modulus);

    if (strcmp(processor, "cpu") == 0)
        rsa_cpu(input_buffer, exponent, modulus, output_buffer);
    if (strcmp(processor, "gpu") == 0)
        rsa_gpu(input_buffer, exponent, modulus, output_buffer);
    
    write_file(output_file_name, output_buffer);

    return 0;
}
