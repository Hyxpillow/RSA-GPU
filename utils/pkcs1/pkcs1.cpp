#include "pkcs1.h"
#include <iomanip>

void save_pad_file(size_t modulus_length,
                   const std::string &filename,
                   unsigned char flag,
                   const std::vector<BIGNUM*> &output_blocks)
{
    std::ofstream file(filename, std::ios::binary);
    if (!file.is_open())
    {
        std::cerr << "Error: cannot open file " << filename << std::endl;
        exit(1);
    }

    unsigned char *buf =(unsigned char*)malloc(1 + modulus_length * sizeof(unsigned char));
    for (const BIGNUM* block : output_blocks)
    {   
        BN_bn2bin(block, buf);

        // if (buf[modulus_length - 1] != 0x00) {
        //     perror("padding beginning error");
        //     exit(1);
        // }
        if (buf[0] != flag) {
            perror("padding flag error");
            exit(1);
        }
        int idx_for_data_begin = 1;
        while (buf[idx_for_data_begin++] != 0);

        for (size_t i = idx_for_data_begin; i < modulus_length - 1; i++) {
            file.write((const char *)(buf + i), 1);
        }
    }
    free(buf);
    file.close();
}

void save_not_pad_file(size_t modulus_length,
                       const std::string &filename,
                       const std::vector<BIGNUM*> &output_blocks)
{
    std::ofstream file(filename, std::ios::binary);
    if (!file.is_open())
    {
        std::cerr << "Error: cannot open file " << filename << std::endl;
        exit(1);
    }

    unsigned char *buf =(unsigned char*)malloc(1 + modulus_length * sizeof(unsigned char));
    for (const BIGNUM* block : output_blocks)
    {   
        BN_bn2bin(block, buf);
        for (size_t i = 0; i < modulus_length; i++) {
            file.write((const char*)buf + i, 1);
        }
    }
    free(buf);

    file.close();
}

std::vector<BIGNUM*> load_and_not_pad_file(size_t modulus_length,
                                          const std::string &filename)
{
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open())
    {
        std::cerr << "Error: cannot open file " << filename << std::endl;
        exit(1);
    }

    size_t block_size = modulus_length;
    std::vector<BIGNUM*> blocks;

    while (!file.eof())
    {
        std::vector<unsigned char> block_data(block_size);
        file.read(reinterpret_cast<char *>(block_data.data()), block_size);
        size_t bytes_read = file.gcount();
        if (bytes_read > 0)
        {
            block_data.resize(bytes_read);

            BIGNUM *bn = BN_new();
            BN_bin2bn(block_data.data(), block_data.size(), bn);
            blocks.push_back(bn);
        }
    }
    
    file.close();
    return blocks;
}

std::vector<BIGNUM*> load_and_pad_file(size_t modulus_length, const std::string &filename, unsigned char flag)
{
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open())
    {
        std::cerr << "Error: cannot open file " << filename << std::endl;
        exit(1);
    }

    if (modulus_length < 11)
    {
        std::cerr << "Error: length must be at least 11" << std::endl;
        exit(1);
    }

    size_t block_size = modulus_length - 11;
    std::vector<BIGNUM*> blocks;

    while (!file.eof())
    {
        std::vector<unsigned char> block_data(block_size);
        file.read(reinterpret_cast<char *>(block_data.data()), block_size);
        size_t bytes_read = file.gcount();
        if (bytes_read > 0)
        {
            block_data.resize(bytes_read);
            auto padded_data = pkcs1_padding(block_data, modulus_length, flag);

            BIGNUM *bn = BN_new();
            BN_bin2bn(padded_data.data(), padded_data.size(), bn);
            blocks.push_back(bn);
        }
    }

    file.close();
    return blocks;
}

std::vector<unsigned char> pkcs1_padding(const std::vector<unsigned char> &data, size_t modulus_length, unsigned char flag)
{
    size_t padding_length = modulus_length - data.size() - 3;
    if (padding_length < 0)
    {
        throw std::runtime_error("padding length is negative");
    }
    std::vector<unsigned char> result(modulus_length);

    result[0] = 0x00;
    result[1] = flag;
    for (size_t i = 2; i < modulus_length - data.size() - 1; ++i) {
        result[i] = 0xff;
    }
    result[modulus_length - data.size() - 1] = 0x00;
    for (size_t i = modulus_length - data.size(); i < modulus_length; i++)
    {
        result[i] = data[i - modulus_length + data.size()];
    }
    return result;
}

// int main()
// {
//     std::string filename = "./test_data.bin";
//     std::string out_filename = "./test_data_out.bin";
//     size_t modulus_length = 32;
//     // std::vector<BigNum> blocks = load_and_not_pad_file(modulus_length, filename);
//     std::vector<BigNum> blocks = load_and_pad_file(modulus_length, filename, 0x01);
//     std::cout << "Loaded " << blocks.size() << " blocks" << std::endl;
//     std::cout << blocks[0].toString() << std::endl;
//     save_pad_file(modulus_length, out_filename, 0x01, blocks);
//     // save_not_pad_file(modulus_length, out_filename, blocks);
//     return 0;
// }
