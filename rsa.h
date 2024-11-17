#include "utils/obn.h"
#include "utils/config.h"
#include <vector>
void do_rsa(const std::vector<BIGNUM*> &input_blocks, 
            std::vector<BIGNUM*> &output_blocks, 
            const BN_CONFIG &bn_config, const char* processor);
