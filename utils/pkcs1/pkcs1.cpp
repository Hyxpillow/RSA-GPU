#include "pkcs1.h"

std::vector<BigNum> load_and_pad_file(size_t module_length, const std::string& filename, unsigned char flag) {
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Error: cannot open file " << filename << std::endl;
        exit(1);
    }

    if (module_length < 11) {
        std::cerr << "Error: length must be at least 11" << std::endl;
        exit(1);
    }

    size_t block_size = module_length - 11;
    std::vector<BigNum> blocks;

    while (!file.eof())
    {
        std::vector<unsigned char> block_data(block_size);
        file.read(reinterpret_cast<char*>(block_data.data()), block_size);
        size_t bytes_read = file.gcount();
        if (bytes_read > 0) {
            block_data.resize(bytes_read);
            auto padded_data = pkcs1_padding(block_data, module_length, flag);
            blocks.push_back(BigNum(std::string(padded_data.begin(), padded_data.end())));
        }
    }

    file.close();
    return blocks;
}

std::vector<unsigned char> pkcs1_padding(const std::vector<unsigned char>& data, size_t module_length, unsigned char flag) {
    size_t padding_length = module_length - data.size() - 3;
    if (padding_length < 0) {
        throw std::runtime_error("padding length is negative");
    }
    std::vector<unsigned char> result(module_length);

    for (size_t i = 0; i < data.size(); ++i) {
        result[i] = data[i];
    }
    result[data.size()] = 0x00;
    for(size_t i = data.size() + 1; i < module_length-2; ++i) {
        result[i] = 0xff;
    }
    result[module_length-2] = flag;
    result[module_length-1] = 0x00;

    // print the result
    for (size_t i = 0; i < module_length; ++i) {
        std::cout << std::hex << std::setw(2) << std::setfill('0') << (int)result[i] << " ";
    }
    std::cout << std::endl;
    return result;
}

int main() {
    std::string filename = "./test_data.bin";
    size_t module_length = 32;
    std::vector<BigNum> blocks = load_and_pad_file(module_length, filename, 0x01);
    std::cout << "Loaded " << blocks.size() << " blocks" << std::endl;
    std::cout << blocks[0].toString() << std::endl;
    return 0;
}

