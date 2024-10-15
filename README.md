# RSA-GPU

generate RSA private key (pkcs1)
```
openssl genrsa 128 -out private.pem 
```

generate RSA public key (pkcs1)
```
openssl rsa -RSAPublicKey_out -in private.pem -out public.pem
```