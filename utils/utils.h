#pragma once

unsigned char* read_file(const char *);
void write_file(const char *, const unsigned char *);
void base64_decode();
void parse_private_key(const unsigned char*, unsigned char*, unsigned char*);
void parse_public_key(const unsigned char*, unsigned char*, unsigned char*);
void rsa_cpu(const unsigned char*, const unsigned char*, const unsigned char*, unsigned char*);
void rsa_gpu(const unsigned char*, const unsigned char*, const unsigned char*, unsigned char*);