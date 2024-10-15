#include "parse_asn1_integer.h"

BigNum parse_asn1_integer(const std::vector<unsigned char>& buffer, size_t& offset) {
    if (buffer[offset] != 0x02) { // ASN.1 integer type tag is 0x02
        std::cout << "Expected integer ASN.1 tag, got " << std::hex << static_cast<int>(buffer[offset]) << std::endl;
        exit(1);
    }
    size_t length = buffer[offset + 1]; 

    BigNum res;
    res.length = buffer[offset + 1];
    for (int i = 0; i < res.length; i++) {
        res.data[i] = buffer[offset + 1 + res.length - i];
    }
    offset += (length + 2); 
    return res;
}