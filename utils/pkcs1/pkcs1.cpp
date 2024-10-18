#include "pkcs1.h"

void save_pad_file(size_t modulus_length,
                   const std::string &filename,
                   unsigned char flag,
                   const std::vector<BigNum> &output_blocks)
{
    std::ofstream file(filename, std::ios::binary);
    if (!file.is_open())
    {
        std::cerr << "Error: cannot open file " << filename << std::endl;
        exit(1);
    }

    for (const BigNum &block : output_blocks)
    {
        if (block.data[modulus_length - 1] != 0x00) {
            perror("padding beginning error");
            exit(1);
        }
        if (block.data[modulus_length - 2] != flag) {
            perror("padding flag error");
            exit(1);
        }
        int idx_for_data_begin = modulus_length - 3;
        while (block.data[idx_for_data_begin--] != 0);
        for (size_t i = 0; i <= idx_for_data_begin; i++) {
            file.write((const char *)(block.data + idx_for_data_begin - i), 1);
        }
    }

    file.close();
}

void save_not_pad_file(size_t modulus_length,
                       const std::string &filename,
                       const std::vector<BigNum> &output_blocks)
{
    std::ofstream file(filename, std::ios::binary);
    if (!file.is_open())
    {
        std::cerr << "Error: cannot open file " << filename << std::endl;
        exit(1);
    }

    for (const BigNum &block : output_blocks)
    {
        for (size_t i = 0; i < modulus_length; i++) {
            file.write((const char *)(block.data + modulus_length - 1 - i), 1);
        }
    }

    file.close();
}

std::vector<BigNum> load_and_not_pad_file(size_t modulus_length,
                                          const std::string &filename)
{
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open())
    {
        std::cerr << "Error: cannot open file " << filename << std::endl;
        exit(1);
    }

    size_t block_size = modulus_length;
    std::vector<BigNum> blocks;

    while (!file.eof())
    {
        std::vector<unsigned char> block_data(block_size);
        file.read(reinterpret_cast<char *>(block_data.data()), block_size);
        size_t bytes_read = file.gcount();
        if (bytes_read > 0)
        {
            block_data.resize(bytes_read);
            blocks.push_back(BigNum(block_data.data(), block_data.size()));
        }
    }
    
    file.close();
    return blocks;
}

std::vector<BigNum> load_and_pad_file(size_t modulus_length, const std::string &filename, unsigned char flag)
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
    std::vector<BigNum> blocks;

    while (!file.eof())
    {
        std::vector<unsigned char> block_data(block_size);
        file.read(reinterpret_cast<char *>(block_data.data()), block_size);
        size_t bytes_read = file.gcount();
        if (bytes_read > 0)
        {
            block_data.resize(bytes_read);
            auto padded_data = pkcs1_padding(block_data, modulus_length, flag);
            blocks.push_back(BigNum(padded_data.data(), padded_data.size()));
            std::cout << blocks[blocks.size() - 1].length << std::endl;
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

    // print the result
    for (size_t i = 0; i < modulus_length; ++i)
    {
        std::cout << std::hex << std::setw(2) << std::setfill('0') << (int)result[i] << " ";
    }
    std::cout << std::endl;
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
