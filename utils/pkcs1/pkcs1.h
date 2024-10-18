#pragma once
#include "big_num.h"
#include <iostream>
#include <fstream> // Add this line
#include <string>
#include <vector>

void save_not_pad_file(size_t modulus_length,
                       const std::string &filename,
                       const std::vector<BigNum> &output_blocks);

void save_pad_file(size_t modulus_length,
                   const std::string &filename,
                   unsigned char flag,
                   const std::vector<BigNum> &output_blocks);

std::vector<BigNum> load_and_not_pad_file(size_t modulus_length,
                                          const std::string &filename);
std::vector<BigNum> load_and_pad_file(size_t modulus_length,
                                      const std::string &filename,
                                      unsigned char flag);
std::vector<unsigned char> pkcs1_padding(const std::vector<unsigned char> &data,
                                         size_t modulus_length,
                                         unsigned char flag);