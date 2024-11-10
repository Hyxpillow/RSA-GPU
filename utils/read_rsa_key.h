#include <openssl/rsa.h>
#include <openssl/pem.h>

RSA *read_rsa_key(const char *key_file_name);