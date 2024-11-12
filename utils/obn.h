#pragma once
#include <openssl/bn.h>
#define OBN_MAX_NUM_BYTES 512

struct OURBIGNUM {
    unsigned char data[OBN_MAX_NUM_BYTES];
};

OURBIGNUM* OBN_new();
void OBN_free(OURBIGNUM*);

void OBN_add(OURBIGNUM *r, const OURBIGNUM *a, const OURBIGNUM *b);
void OBN_sub(OURBIGNUM *r, const OURBIGNUM *a, const OURBIGNUM *b);
void OBN_mul(OURBIGNUM *r, const OURBIGNUM *a, const OURBIGNUM *b);
void OBN_div(OURBIGNUM *dv, OURBIGNUM *rem, const OURBIGNUM *m, const OURBIGNUM *d);
# define OBN_mod(rem,m,d,ctx) OBN_div(NULL,(rem),(m),(d),(ctx))

int OBN_lshift(OURBIGNUM *r, const OURBIGNUM *a, int n);
int OBN_rshift(OURBIGNUM *r, const OURBIGNUM *a, int n);

int OBN_cmp(const OURBIGNUM *a, const OURBIGNUM *b);
int OBN_is_zero(const OURBIGNUM *a);

void OBN_obn2bn(BIGNUM** a, const OURBIGNUM *b);
void OBN_bn2obn(OURBIGNUM *a, const BIGNUM* b);

void OBN_obn2hex(const OURBIGNUM *a, char *str);
void OBN_hex2obn(OURBIGNUM *a, const char *str);