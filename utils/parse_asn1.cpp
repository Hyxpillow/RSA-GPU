#include "parse_asn1.h"

void parse_asn1_integer(const std::vector<unsigned char>& buffer, size_t& offset, BIGNUM* bn) {
    if (buffer[offset] != 0x02) { // ASN.1 integer type tag is 0x02
        std::cout << "Expected integer ASN.1 tag, got " << std::hex << static_cast<int>(buffer[offset]) << std::endl;
        exit(1);
    }

    size_t length, num_bytes_for_length;
    get_length_and_num_bytes_for_length(buffer, offset, length, num_bytes_for_length);

    offset = offset + 2 + num_bytes_for_length;
    BN_bin2bn(&buffer[offset], length, bn);
    offset += length;
}

void skip_asn1_sequence_header(const std::vector<unsigned char>& buffer, size_t& offset) {
    if (buffer[offset] != 0x30) { // ASN.1 sequence type tag is 0x30
        std::cout << "Expected sequence ASN.1 tag, got " << std::hex << static_cast<int>(buffer[offset]) << std::endl;
        exit(1);
    }
    size_t length, num_bytes_for_length;
    get_length_and_num_bytes_for_length(buffer, offset, length, num_bytes_for_length);
    offset = offset + 2 + num_bytes_for_length;
}

void skip_asn1_sequence(const std::vector<unsigned char>& buffer, size_t& offset) {
    if (buffer[offset] != 0x30) { // ASN.1 sequence type tag is 0x30
        std::cout << "Expected sequence ASN.1 tag, got " << std::hex << static_cast<int>(buffer[offset]) << std::endl;
        exit(1);
    }
    size_t length, num_bytes_for_length;
    get_length_and_num_bytes_for_length(buffer, offset, length, num_bytes_for_length);
    offset = offset + 2 + num_bytes_for_length + length;
}

void skip_asn1_integer(const std::vector<unsigned char>& buffer, size_t& offset) {
    if (buffer[offset] != 0x02) { // ASN.1 integer type tag is 0x02
        std::cout << "Expected integer ASN.1 tag, got " << std::hex << static_cast<int>(buffer[offset]) << std::endl;
        exit(1);
    }
    size_t length, num_bytes_for_length;
    get_length_and_num_bytes_for_length(buffer, offset, length, num_bytes_for_length);
    offset = offset + 2 + num_bytes_for_length + length;
}

void skip_asn1_octet_string_header(const std::vector<unsigned char>& buffer, size_t& offset) 
{
    if (buffer[offset] != 0x04) { // ASN.1 octet string type tag is 0x04
        std::cout << "Expected octet string ASN.1 tag, got " << std::hex << static_cast<int>(buffer[offset]) << std::endl;
        exit(1);
    }
    size_t length, num_bytes_for_length;
    get_length_and_num_bytes_for_length(buffer, offset, length, num_bytes_for_length);
    offset = offset + 2 + num_bytes_for_length;
}
void skip_asn1_bit_string_header(const std::vector<unsigned char>& buffer, size_t& offset)
{
    if (buffer[offset] != 0x03) { // ASN.1 bit string type tag is 0x03
        std::cout << "Expected bit string ASN.1 tag, got " << std::hex << static_cast<int>(buffer[offset]) << std::endl;
        exit(1);
    }
    size_t length, num_bytes_for_length;
    get_length_and_num_bytes_for_length(buffer, offset, length, num_bytes_for_length);
    offset = offset + 2 + num_bytes_for_length + 1;
}

void get_length_and_num_bytes_for_length(const std::vector<unsigned char>& buffer, 
                                        size_t& offset,
                                        size_t& length,
                                        size_t& num_bytes_for_length)
{
    size_t second_byte = buffer[offset + 1]; 
    length = 0;
    num_bytes_for_length = 0;

    if ((second_byte & 0x7F) == second_byte) {
        length = second_byte;
    } else {
        num_bytes_for_length = second_byte & 0x7F;
        for (size_t i = 0; i < num_bytes_for_length; i++) {
            length <<= 8;
            length += buffer[offset + 2 + i];
        }
    }
}
