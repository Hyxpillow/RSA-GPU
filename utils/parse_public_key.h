#pragma once
#include <openssl/bn.h>

bool is_public_key(std::vector<unsigned char>&);
void parse_public_key(std::vector<unsigned char>&, BIGNUM*, BIGNUM*);