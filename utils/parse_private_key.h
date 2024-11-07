#pragma once
#include <openssl/bn.h>
#include <vector>

bool is_private_key(std::vector<unsigned char>&);
void parse_private_key(std::vector<unsigned char>&, BIGNUM*, BIGNUM*);