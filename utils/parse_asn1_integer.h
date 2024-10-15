#include <vector>
#include <iostream>
#include "big_num.h"

BigNum parse_asn1_integer(const std::vector<unsigned char>& buffer, size_t& offset);