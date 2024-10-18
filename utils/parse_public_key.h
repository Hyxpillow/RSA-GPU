#pragma once
#include "big_num.h"

bool is_public_key(std::vector<unsigned char>&);
void parse_public_key(std::vector<unsigned char>&, BigNum&, BigNum&);