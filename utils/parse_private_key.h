#pragma once
#include "big_num.h"

bool is_private_key(std::vector<unsigned char>&);
void parse_private_key(std::vector<unsigned char>&, BigNum&, BigNum&);