#include <openssl/bn.h>
#include <vector>
void rsa_cpu(const std::vector<BIGNUM*>&, const BIGNUM*, const BIGNUM*, std::vector<BIGNUM*>&);
