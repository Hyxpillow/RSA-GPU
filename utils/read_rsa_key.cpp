#include <openssl/rsa.h>
#include <openssl/pem.h>

RSA *read_rsa_key(const char *key_file_name)
{
    FILE *fp = fopen(key_file_name, "rb");
    if (!fp)
    {
        perror("Failed to open key file");
        exit(1);
    }
    // IF PUBLIC KEY
    RSA *rsa = PEM_read_RSA_PUBKEY(fp, NULL, NULL, NULL);
    fclose(fp);
    if (rsa)
    {
        return rsa;
    }
    // IF PRIVATE KEY
    fp = fopen(key_file_name, "rb");
    EVP_PKEY *pkey = PEM_read_PrivateKey(fp, NULL, NULL, NULL);
    fclose(fp);
    if (!pkey)
    {
        printf("Failed to parse PKCS#8 key\n");
        exit(1);
    }
    rsa = EVP_PKEY_get1_RSA(pkey);
    EVP_PKEY_free(pkey);
    return rsa;
}
