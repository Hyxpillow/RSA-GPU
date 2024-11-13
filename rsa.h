#include "utils/obn.h"
#include "utils/config.h"
#include <vector>
void do_rsa(const std::vector<OURBIGNUM> &input_blocks, 
            std::vector<OURBIGNUM> &output_blocks, 
            const BN_CONFIG &bn_config, const ModType type);
