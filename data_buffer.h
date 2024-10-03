#pragma once
#include <utility>

class DataBuffer {
public:
    DataBuffer operator*(const DataBuffer& other) const;
    DataBuffer operator%(const DataBuffer& other) const;
    DataBuffer operator!=(const DataBuffer& other) const;
    DataBuffer operator>>(const DataBuffer& other) const;
private:
    unsigned char* data;
    size_t length;
};