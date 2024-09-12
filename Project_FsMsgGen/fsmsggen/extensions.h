#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/ec.h>
#include <openssl/pem.h>
#include <openssl/sha.h>
#include <openssl/ecdsa.h> 

#include "../liboqs/build/include/oqs/sig_dilithium.h"
#include "./cshared/cstr.h"

#define CERT_MAX_SIZE 0x10000
#define sha256_hash_size 32

extern int flag_PQC;  

int sha256_calculate(char *hash, const char *ptr, size_t len);
void *search_private_Dilithium_key(char* path, const char *sName);
void *search_public_Dilithium_key(char* _keyPath);
//int OCTET_STRING_fromBuf(OCTET_STRING_t *st, const char *str, int len);