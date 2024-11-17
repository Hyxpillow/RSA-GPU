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

Compile modexptest
```
g++ -o modexptest modexptest.cpp modexp/modexp_mong_cpu.cpp modexp/modexp_base.cpp utils/obn.cpp utils/config.cpp  -I ./modexp -I ./utils/ -lssl -lcrypto
```

Compile 
```
nvcc -o project main.cpp rsa.cpp modexp/*.cpp utils/*.cpp utils/*.cu -I ./modexp -I ./utils/ -lssl -lcrypto
```