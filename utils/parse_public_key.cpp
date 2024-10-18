#include <iostream>
#include "big_num.h"
#include <vector>
#include "base64_decode.h"
#include "parse_asn1.h"

bool is_public_key(std::vector<unsigned char>& buffer)
{
    const std::string begin_marker = "-----BEGIN PUBLIC KEY-----";
    const std::string end_marker = "-----END PUBLIC KEY-----";
    std::string buffer_str(buffer.begin(), buffer.end());

    size_t begin_pos = buffer_str.find(begin_marker);
    if (begin_pos == std::string::npos) {
        std::cerr << "BEGIN marker not found\n";
        return false;
    }
    size_t end_pos = buffer_str.find(end_marker, begin_pos);
    if (end_pos == std::string::npos) {
        std::cerr << "END marker not found\n";
        return false;
    }
    return true;
}

std::vector<unsigned char> skip_begin_and_end_maker_public_key(const std::vector<unsigned char>& buffer) {
    const std::string begin_marker = "-----BEGIN PUBLIC KEY-----";
    const std::string end_marker = "-----END PUBLIC KEY-----";

    // Convert buffer to string for easier manipulation
    std::string buffer_str(buffer.begin(), buffer.end());

    // Find the BEGIN marker
    size_t begin_pos = buffer_str.find(begin_marker);
    if (begin_pos == std::string::npos) {
        std::cerr << "BEGIN marker not found\n";
        exit(1);
    }

    // Find the END marker
    size_t end_pos = buffer_str.find(end_marker, begin_pos);
    if (end_pos == std::string::npos) {
        std::cerr << "END marker not found\n";
        exit(1);
    }

    // Skip the BEGIN marker and go to the Base64 data
    size_t base64_start = begin_pos + begin_marker.size();

    // Extract the Base64 data (exclude END marker)
    std::string base64_data = buffer_str.substr(base64_start, end_pos - base64_start);

    // Remove newlines and spaces from the Base64 data
    base64_data.erase(remove(base64_data.begin(), base64_data.end(), '\n'), base64_data.end());
    base64_data.erase(remove(base64_data.begin(), base64_data.end(), '\r'), base64_data.end());
    base64_data.erase(remove(base64_data.begin(), base64_data.end(), ' '), base64_data.end());

    // Convert Base64 string back to vector<unsigned char>
    std::vector<unsigned char> result(base64_data.begin(), base64_data.end());
    return result;
}

void parse_public_key(std::vector<unsigned char>& key_file_buffer,
                         BigNum& exponent, 
                         BigNum& modulus)
{    
    std::vector<unsigned char> base64_data = skip_begin_and_end_maker_public_key(key_file_buffer);
    std::vector<unsigned char> key_buffer = base64_decode(base64_data);

    size_t offset = 0;
    
    skip_asn1_sequence_header(key_buffer, offset);
    skip_asn1_sequence(key_buffer, offset);
    skip_asn1_bit_string_header(key_buffer, offset);
    skip_asn1_sequence_header(key_buffer, offset);

    modulus = parse_asn1_integer(key_buffer, offset);
    exponent = parse_asn1_integer(key_buffer, offset);

    std::cout << "modulus " << modulus.toString() << std::endl;
    std::cout << "public_exponent " << exponent.toString() << std::endl;
}
