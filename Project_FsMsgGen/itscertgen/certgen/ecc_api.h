/*********************************************************************
######################################################################
##
##  Created by: Denis Filatov
##
##  Copyleft (c) 2015
##  This code is provided under the CeCill-C license agreement.
######################################################################
*********************************************************************/
#ifndef ecc_api_h
#define ecc_api_h

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif



typedef enum {
	ecies_nistp256,
	ecies_brainpoolp256r,
	ecies_brainpoolp384r,
	ecies_nistp384,
	ecies_sm2
}ecc_curve_id;

int ecc_api_init();
int ecc_api_done();
const char* ecc_curve_name(ecc_curve_id curve);
typedef enum {
	sha_256,
	sha_384,
	sm3
}ecc_hash_id;

typedef enum {
	aes_128_ccm
}ecc_sym_algorithm;

typedef enum {
	ecc_bin,
	ecc_hex,
	ecc_pem
}ecc_format;

typedef enum {
	ecc_x_only = 0,
	ecc_compressed_y0 = 2,
	ecc_compressed_y1 = 3,
	ecc_uncompressed = 4
}ecc_point_type;

extern const char sha256_emptyString[];
extern const char sha384_emptyString[];
extern const char sm3_emptyString[];

void * ecc_key_gen(ecc_curve_id pk_alg);
void * ecc_key_init(ecc_curve_id pk_alg, const char* pkey);
void   ecc_key_free(void*);
int    ecc_key_private(void*, char* buf);
int    ecc_key_public(void*, char* x, char* y, int * psign);
int    ecc_key_private_save(void*, const char* path, ecc_format format);
void * ecc_key_private_load(const char* path, ecc_curve_id pk_alg);
int    ecc_key_public_save(void*, const char* path, ecc_format format);
void * ecc_key_public_load(const char* path, ecc_curve_id pk_alg);
void * ecc_key_public_set(ecc_curve_id pk_alg, ecc_point_type ptype, const char * px, const char * py);

#define sha256_hash_size 32
#define sha384_hash_size 48
#define sm3_hash_size 32

int sha256_calculate(char* hash, const char * ptr, size_t len);
int sha384_calculate(char* hash, const char * ptr, size_t len);
int sm3_calculate(char* hash, const char * ptr, size_t len);

void * ecc_recvalue(void * req, char * px, char * py, int * psign);
void * ecc_reconstruct(void * req, void * eph, void * ca, unsigned char * H);

int    ecc_sign(void * key, const char * hash, int hlength, char *r, char *s);
int    ecc_verify(void * key, const char * hash, int hlength, const char *r, const char *s);

#ifdef __cplusplus
}
#endif
#endif
