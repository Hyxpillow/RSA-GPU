#include <iostream>
#include <vector>
#include "base64_decode.h"

static const signed char base64TableMap[] = {
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 62, -1, -1, -1, 63,
    52, 53, 54, 55, 56, 57, 58, 59, 60, 61, -1, -1, -1, -1, -1, -1,
    -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17,
    18, 19, 20, 21, 22, 23, 24, 25, -1, -1, -1, -1, -1, -1, 26, 27,
    28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43,
    44, 45, 46, 47, 48, 49, 50, 51, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};


std::vector<unsigned char> base64_decode(const std::vector<unsigned char>& input) {
    std::vector<unsigned char> output;
    int pos = 0;
    int n = 0;
    unsigned char cch, t;
    unsigned char currentByte = 0;

    for (size_t i = 0; i < input.size(); ++i) {
        cch = input[i];
        t = base64TableMap[cch];
        if (cch == '=') {
            break;
        }

        switch (pos) {
            case 0:
                currentByte = (t << 2) & 0xff;
                pos = 1;
                break;
            case 1:
                currentByte |= ((t & 0x30) >> 4) & 0xff;
                output.push_back(currentByte);
                currentByte = ((t & 0xf) << 4) & 0xff;
                pos = 2;
                break;
            case 2:
                currentByte |= ((t & 0x3c) >> 2) & 0xff;
                output.push_back(currentByte);
                currentByte = ((t & 0x3) << 6) & 0xff;
                pos = 3;
                break;
            case 3:
                currentByte |= t & 0xff;
                output.push_back(currentByte);
                pos = 0;
                break;
        }
    }

    return output;
}