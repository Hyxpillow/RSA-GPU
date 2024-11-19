#include "utils/obn.h"
#include <vector>
void do_rsa(const std::vector<BIGNUM*> &input_blocks, 
            std::vector<BIGNUM*> &output_blocks, 
            const BIGNUM* exponent,
            const BIGNUM* modulus,
            const char* processor);
