#include <openssl/bn.h>

void parse_key(const char* key_file_name, 
                BIGNUM* _exponent, 
                BIGNUM* _modulus);