#pragma once
#include "big_num.h"
#include <iostream>
#include <fstream> // Add this line
#include <string>
#include <vector>

std::vector<BigNum> load_and_pad_file(size_t module_length, const std::string& filename, unsigned char flag);
std::vector<unsigned char> pkcs1_padding(const std::vector<unsigned char>& data, size_t module_length, unsigned char flag);