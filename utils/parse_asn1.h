#include <vector>
#include <iostream>
#include "big_num.h"

void get_length_and_num_bytes_for_length(const std::vector<unsigned char>& buffer, 
                                        size_t& offset,
                                        size_t& length,
                                        size_t& num_bytes_for_length);

BigNum parse_asn1_integer(const std::vector<unsigned char>& buffer, size_t& offset);
void skip_asn1_sequence_header(const std::vector<unsigned char>& buffer, size_t& offset);
void skip_asn1_sequence(const std::vector<unsigned char>& buffer, size_t& offset);
void skip_asn1_integer(const std::vector<unsigned char>& buffer, size_t& offset);
void skip_asn1_octet_string_header(const std::vector<unsigned char>& buffer, size_t& offset);
void skip_asn1_bit_string_header(const std::vector<unsigned char>& buffer, size_t& offset);