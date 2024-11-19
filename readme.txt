Machine:
    cuda2

Compile:
    nvcc -o rsa main.cpp rsa.cpp modexp/*.cpp utils/*.cpp utils/*.cu -I ./modexp -I ./utils/ -lssl -lcrypto -w

Run:
    ./rsa <encrypt|decrypt> <keyfile> <inputfile> <outputfile> <cpu|gpu>

Run Encryption with given Public Key:
    ./rsa encrypt public.pem in.txt out.enc gpu

Run Decryption with given Private Key:
    ./rsa decrypt private.pem out.enc out.dec gpu

Generate your own Private Key:
    openssl genrsa -out private.pem 512
    openssl pkey -in private.pem -pubout -out public.pem

Transform pkcs1 to pkcs8 (No need on cuda2)
```
openssl pkcs8 -topk8 -in private.pem -out private2.pem -nocrypt
```