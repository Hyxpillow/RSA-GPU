# RSA-GPU

generate RSA private key (pkcs1)
```
openssl genrsa -out private.pem 512
```

generate RSA public key (pkcs1)
```
openssl pkey -in private.pem -pubout -out public.pem
```

Transform pkcs1 to pkcs8
```
openssl pkcs8 -topk8 -in private.pem -out private2.pem -nocrypt
```