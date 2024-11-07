#include "parse_key.h"
#include <iostream>
#include <vector>
#include "parse_private_key.h"
#include "parse_public_key.h"

void parse_key(const char* key_file_name, 
                BIGNUM* _exponent, 
                BIGNUM* _modulus) {
    FILE *key_file = fopen(key_file_name, "rb");
    if (!key_file) {
        perror("Failed to open key file");
        exit(1);
    }

    fseek(key_file, 0, SEEK_END);
    long key_file_size = ftell(key_file);
    rewind(key_file);
    
    std::vector<unsigned char> key_file_buffer(key_file_size);
    fread(key_file_buffer.data(), 1, key_file_size, key_file);
    fclose(key_file);


    if (is_private_key(key_file_buffer)) {
        parse_private_key(key_file_buffer, _exponent, _modulus);
    } else if (is_public_key(key_file_buffer)) {
        parse_public_key(key_file_buffer, _exponent, _modulus);
    } else {
        perror("Invalid Key");
        exit(1);
    }
}