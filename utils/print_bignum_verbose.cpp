#include "print_bignum_verbose.h"

void BN_print_verbose(const char* name, BIGNUM *bn)
{
    printf("%s %dBytes ", name, BN_num_bytes(bn));
    BN_print_fp(stdout, bn);
    printf("\n");
}