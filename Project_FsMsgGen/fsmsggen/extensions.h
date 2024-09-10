#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/ec.h>
#include <openssl/pem.h>
#include <openssl/sha.h>
#include <openssl/ecdsa.h> 

#include "../liboqs/build/include/oqs/sig_dilithium.h"
#include "./cshared/cstr.h"
extern int flag_PQC;  

int sha256_calculate(char *hash, const char *ptr, size_t len);
void *search_private_Dilithium_key(char* path, const char *sName);