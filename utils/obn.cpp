#include "obn.h"
#include <string.h>
#include <stdlib.h>

OURBIGNUM* OBN_new()
{
    OURBIGNUM *r = (OURBIGNUM*)malloc(sizeof(OURBIGNUM));
    return r;
}
void OBN_free(OURBIGNUM* a)
{
    free(a);
}

void OBN_add(OURBIGNUM *r, const OURBIGNUM *a, const OURBIGNUM *b)
{
    unsigned int carry = 0;
    for (int i = 0; i < OBN_MAX_NUM_BYTES; ++i) {
        unsigned int sum = a->data[i] + b->data[i] + carry;
        r->data[i] = sum & 0xFF;
        carry = sum >> 8;
    }
}
void OBN_sub(OURBIGNUM *r, const OURBIGNUM *a, const OURBIGNUM *b)
{
    int borrow = 0;
    for (int i = 0; i < OBN_MAX_NUM_BYTES; ++i) {
        int diff = a->data[i] - b->data[i] - borrow;
        if (diff < 0) {
            diff += 256;
            borrow = 1;
        } else {
            borrow = 0;
        }
        r->data[i] = diff & 0xFF;
    }
}
void OBN_mul(OURBIGNUM *r, const OURBIGNUM *a, const OURBIGNUM *b)
{
    memset(r->data, 0, OBN_MAX_NUM_BYTES);
    for (size_t i = 0; i < OBN_MAX_NUM_BYTES; ++i)
    {
        unsigned int carry = 0;
        for (size_t j = 0; j < OBN_MAX_NUM_BYTES - i; ++j)
        {
            unsigned int product =
                r->data[i + j] +
                a->data[i] * b->data[j] +
                carry;

            r->data[i + j] = product & 0xFF;
            carry = product >> 8;
        }
    }
}
void OBN_div(OURBIGNUM *dv, OURBIGNUM *rem, const OURBIGNUM *m, const OURBIGNUM *d)
{   
    if (dv)
        memset(dv->data, 0, OBN_MAX_NUM_BYTES);
    memcpy(rem->data, m->data, OBN_MAX_NUM_BYTES);
    // 获取被除数的实际位数（去除前导零）
    int m_bits = OBN_MAX_NUM_BYTES * 8;
    while (m_bits > 0 && ((rem->data[(m_bits - 1) / 8] >> ((m_bits - 1) % 8)) & 1) == 0) {
        m_bits--;
    }
    // 获取除数的实际位数（去除前导零）
    int d_bits = OBN_MAX_NUM_BYTES * 8;
    while (d_bits > 0 && ((d->data[(d_bits - 1) / 8] >> ((d_bits - 1) % 8)) & 1) == 0) {
        d_bits--;
    }

    // 如果被除数的位数小于除数，直接返回余数为被除数，商为0
    if (m_bits < d_bits) {
        return;
    }

    int shift = m_bits - d_bits;

    // 临时变量用于保存当前的除数（需要左移）
    OURBIGNUM temp_d;
    memset(temp_d.data, 0, OBN_MAX_NUM_BYTES);

    // 将除数左移以对齐被除数的最高位
    for (int i = d_bits - 1; i >= 0; --i) {
        if ((d->data[i / 8] >> (i % 8)) & 1) {
            int bit_pos = i + shift;
            temp_d.data[bit_pos / 8] |= (1 << (bit_pos % 8));
        }
    }

    // 逐位计算商，从最高位到最低位
    for (int i = shift; i >= 0; --i) {
        // 检查余数是否大于等于当前的除数
        int greater_or_equal = 1;
        for (int j = OBN_MAX_NUM_BYTES - 1; j >= 0; --j) {
            if (rem->data[j] > temp_d.data[j]) {
                greater_or_equal = 1;
                break;
            } else if (rem->data[j] < temp_d.data[j]) {
                greater_or_equal = 0;
                break;
            }
        }

        // 如果余数大于等于当前的除数，则进行减法，并将商的对应位设为1
        if (greater_or_equal) {
            unsigned int borrow = 0;
            for (int j = 0; j < OBN_MAX_NUM_BYTES; ++j) {
                unsigned int sub = rem->data[j] - temp_d.data[j] - borrow;
                borrow = (rem->data[j] < temp_d.data[j] + borrow) ? 1 : 0;
                rem->data[j] = sub & 0xFF;
            }
            if (dv)
                dv->data[i / 8] |= (1 << (i % 8));
        }

        // 将除数右移一位，准备处理下一位
        for (int j = 0; j < OBN_MAX_NUM_BYTES - 1; ++j) {
            temp_d.data[j] = (temp_d.data[j] >> 1) | ((temp_d.data[j + 1] & 1) << 7);
        }
        temp_d.data[OBN_MAX_NUM_BYTES - 1] >>= 1;
    }
}

int OBN_lshift(OURBIGNUM *r, const OURBIGNUM *a, int n)
{
    memset(r->data, 0, OBN_MAX_NUM_BYTES);
    int byte_shift = n / 8;
    int bit_shift = n % 8;
    unsigned char carry = 0;
    
    for (int i = 0; i < OBN_MAX_NUM_BYTES - byte_shift; ++i) {
        unsigned int current = a->data[i];
        r->data[i + byte_shift] = (current << bit_shift) | carry;
        carry = (current >> (8 - bit_shift)) & 0xFF;
    }
    
    return 0;
}
int OBN_rshift(OURBIGNUM *r, const OURBIGNUM *a, int n)
{
    memset(r->data, 0, OBN_MAX_NUM_BYTES);
    int byte_shift = n / 8;
    int bit_shift = n % 8;
    unsigned char carry = 0;
    
    for (int i = OBN_MAX_NUM_BYTES - 1; i >= byte_shift; --i) {
        unsigned int current = a->data[i];
        r->data[i - byte_shift] = (current >> bit_shift) | carry;
        carry = (current << (8 - bit_shift)) & 0xFF;
    }
    return 0;
}

int OBN_cmp(const OURBIGNUM *a, const OURBIGNUM *b)
{
    for (size_t i = OBN_MAX_NUM_BYTES - 1; i >= 0; --i) {
        if (a->data[i] != b->data[i]) {
            return a->data[i] > b->data[i] ? 1 : -1;
        }
    }
    return 0;
}
int OBN_is_zero(const OURBIGNUM *a)
{
    for (int i = 0; i < OBN_MAX_NUM_BYTES; i++)
        if (a->data[i] != 0)
            return 0;
    return 1;
}

void OBN_obn2bn(BIGNUM** a, const OURBIGNUM *b)
{
    char str[OBN_MAX_NUM_BYTES];
    OBN_obn2hex(b, str);
    BN_hex2bn(a, str);
}
void OBN_bn2obn(OURBIGNUM *a, const BIGNUM* b)
{
    char* bn_hex = BN_bn2hex(b);
    OBN_hex2obn(a, bn_hex);
}

void OBN_hex2obn(OURBIGNUM *a, const char *str)
{
    memset(a->data, 0, OBN_MAX_NUM_BYTES);
    size_t len = strlen(str);

    for (size_t i = 0; i < len; ++i) {
        char c = str[len - 1 - i];
        unsigned char value;

        if (c >= '0' && c <= '9') {
            value = c - '0';
        } else if (c >= 'a' && c <= 'f') {
            value = c - 'a' + 10;
        } else if (c >= 'A' && c <= 'F') {
            value = c - 'A' + 10;
        } else {
            // 非法字符，忽略
            continue;
        }

        size_t byte_pos = i / 2;
        if (i % 2 == 0) {
            a->data[byte_pos] |= value;
        } else {
            a->data[byte_pos] |= (value << 4);
        }
    }
}
void OBN_obn2hex(const OURBIGNUM *a, char *str)
{
    size_t index = 0;
    int leading_zero = 1;

    for (int i = OBN_MAX_NUM_BYTES - 1; i >= 0; --i) {
        unsigned char byte = a->data[i];
        char high = (byte >> 4) & 0xF;
        char low = byte & 0xF;

        if (high || !leading_zero) {
            str[index++] = (high < 10) ? ('0' + high) : ('a' + high - 10);
            leading_zero = 0;
        }
        if (low || !leading_zero) {
            str[index++] = (low < 10) ? ('0' + low) : ('a' + low - 10);
            leading_zero = 0;
        }
    }

    if (leading_zero) {
        // 如果整个数是零
        str[index++] = '0';
    }

    str[index] = '\0';
}

int OBN_num_bits(const OURBIGNUM *a)
{
    for (int i = OBN_MAX_NUM_BYTES - 1; i >= 0; --i) {
        if (a->data[i] != 0) {
            for (int bit = 7; bit >= 0; --bit) {
                if (a->data[i] & (1 << bit)) {
                    return i * 8 + bit + 1;
                }
            }
        }
    }
    return 0;
}
int OBN_is_bit_set(const OURBIGNUM *a, int n)
{
    int byte_index = n / 8;
    int bit_index = n % 8;
    return (a->data[byte_index] & (1 << bit_index)) != 0;
}
void OBN_mod_mul(OURBIGNUM *r, const OURBIGNUM *a, const OURBIGNUM *b, const OURBIGNUM *m)
{
    OURBIGNUM tmp;
    OBN_mul(&tmp, a, b);
    OBN_mod(NULL, r, &tmp, m);
}
void OBN_copy(OURBIGNUM *a, const OURBIGNUM *b)
{
    memcpy(a->data, b->data, OBN_MAX_NUM_BYTES);
}
void OBN_mask_bits(OURBIGNUM *a, int n)
{
    int byte_index = n / 8;
    int bit_index = n % 8;

    // Mask the bits in the byte at byte_index
    if (bit_index != 0) {
        a->data[byte_index] &= (1 << bit_index) - 1;
        byte_index++;
    }

    // Set all bytes after byte_index to 0
    for (int i = byte_index; i < OBN_MAX_NUM_BYTES; ++i) {
        a->data[i] = 0;
    }
}
void OBN_one(OURBIGNUM *a)
{
    memset(a->data, 0, OBN_MAX_NUM_BYTES);
    a->data[0] = 1;
}