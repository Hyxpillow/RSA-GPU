#pragma once
#include <openssl/bn.h>
#define OBN_MAX_NUM_BYTES 1024

struct OURBIGNUM {
    unsigned char data[OBN_MAX_NUM_BYTES];
};

OURBIGNUM* OBN_new();
void OBN_free(OURBIGNUM*);

void OBN_add(OURBIGNUM *r, const OURBIGNUM *a, const OURBIGNUM *b);
void OBN_sub(OURBIGNUM *r, const OURBIGNUM *a, const OURBIGNUM *b);
void OBN_mul(OURBIGNUM *r, const OURBIGNUM *a, const OURBIGNUM *b);
void OBN_div(OURBIGNUM *dv, OURBIGNUM *rem, const OURBIGNUM *m, const OURBIGNUM *d);
# define OBN_mod(dv,rem,m,d) OBN_div((dv),(rem),(m),(d))

int OBN_lshift(OURBIGNUM *r, const OURBIGNUM *a, int n);
int OBN_rshift(OURBIGNUM *r, const OURBIGNUM *a, int n);

int OBN_cmp(const OURBIGNUM *a, const OURBIGNUM *b);
int OBN_is_zero(const OURBIGNUM *a);

void OBN_obn2bn(BIGNUM** a, const OURBIGNUM *b);
void OBN_bn2obn(OURBIGNUM *a, const BIGNUM* b);

void OBN_obn2hex(const OURBIGNUM *a, char *str);
void OBN_hex2obn(OURBIGNUM *a, const char *str);

int OBN_num_bits(const OURBIGNUM *a);
int OBN_is_bit_set(const OURBIGNUM *a, int n);
void OBN_mod_mul(OURBIGNUM *r, const OURBIGNUM *a, const OURBIGNUM *b, const OURBIGNUM *m);
void OBN_copy(OURBIGNUM *a, const OURBIGNUM *b);
void OBN_mask_bits(OURBIGNUM *a, int n);
void OBN_one(OURBIGNUM *a);

struct OBN_MUL_GPU_CTX {
    unsigned char a[OBN_MAX_NUM_BYTES];
    unsigned char b[OBN_MAX_NUM_BYTES];

    int buf[OBN_MAX_NUM_BYTES];

    unsigned char low8[OBN_MAX_NUM_BYTES];
    unsigned char mid8[OBN_MAX_NUM_BYTES];
    unsigned char high8[OBN_MAX_NUM_BYTES];

    unsigned char x[OBN_MAX_NUM_BYTES];
    unsigned char y[OBN_MAX_NUM_BYTES];
    unsigned char z[OBN_MAX_NUM_BYTES];
    unsigned char carry[OBN_MAX_NUM_BYTES];

    unsigned char result[OBN_MAX_NUM_BYTES];
};

OBN_MUL_GPU_CTX* OBN_MUL_GPU_CTX_new();
void OBN_MUL_GPU_CTX_free(OBN_MUL_GPU_CTX*);
void OBN_mul_gpu(OURBIGNUM *rd, const OURBIGNUM *ad, const OURBIGNUM *bd, OBN_MUL_GPU_CTX *ctx);