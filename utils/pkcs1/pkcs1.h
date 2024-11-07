#pragma once
#include <openssl/bn.h>
#include <iostream>
#include <fstream> // Add this line
#include <string>
#include <vector>

void save_not_pad_file(size_t modulus_length,
                       const std::string &filename,
                       const std::vector<BIGNUM*> &output_blocks);

void save_pad_file(size_t modulus_length,
                   const std::string &filename,
                   unsigned char flag,
                   const std::vector<BIGNUM*> &output_blocks);

std::vector<BIGNUM*> load_and_not_pad_file(size_t modulus_length,
                                          const std::string &filename);
std::vector<BIGNUM*> load_and_pad_file(size_t modulus_length,
                                      const std::string &filename,
                                      unsigned char flag);
std::vector<unsigned char> pkcs1_padding(const std::vector<unsigned char> &data,
                                         size_t modulus_length,
                                         unsigned char flag);