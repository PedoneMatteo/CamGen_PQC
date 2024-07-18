/*********************************************************************
######################################################################
##
##  Created by: Denis Filatov
##
##  Copyleft (c) 2015
##  This code is provided under the CeCill-C license agreement.
######################################################################
*********************************************************************/
#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/ec.h>
#include <openssl/pem.h>
#include <openssl/sha.h>
#include <openssl/ecdsa.h>
#include <string.h>

#include "ecc_api.h"

#define FIELD_SIZE 32

#define ARRAYSIZE(A) (sizeof(A)/sizeof(A[0]))

#if OPENSSL_VERSION_NUMBER < 0x10101000L
#define EC_POINT_get_affine_coordinates EC_POINT_get_affine_coordinates_GFp
#define EC_POINT_set_affine_coordinates EC_POINT_set_affine_coordinates_GFp
#define EC_POINT_set_compressed_coordinates EC_POINT_set_compressed_coordinates_GFp
void ECDSA_SIG_get0(const ECDSA_SIG *sig, const BIGNUM **pr, const BIGNUM **ps)
{                                                                                                                                                                                                                      if (pr != NULL)
        *pr = sig->r;
    if (ps != NULL)
        *ps = sig->s;
}

int ECDSA_SIG_set0(ECDSA_SIG *sig, BIGNUM *r, BIGNUM *s)
{
    if (r == NULL || s == NULL)
        return 0;
    BN_clear_free(sig->r);
    BN_clear_free(sig->s);
    sig->r = r;
    sig->s = s;
    return 1;
}
#endif
char * _bin2hex(char * hex, size_t hlen, const char * bin, size_t blen);

typedef struct {
	int nid;
	const char * name;
	size_t  fsize;
}ecc_api_curve_data;

static ecc_api_curve_data _config[] = {
	{
		NID_X9_62_prime256v1, SN_X9_62_prime256v1, 32
	},
	{
		NID_brainpoolP256r1, SN_brainpoolP256r1, 32
	},
	{
		NID_brainpoolP384r1, SN_brainpoolP384r1, 48
	},
	{
		NID_secp384r1, SN_secp384r1, 48
	},
	{
		NID_sm2, SN_sm2, 32
	},
};

static EC_GROUP * _curves[ARRAYSIZE(_config)] = { NULL };

const char sha256_emptyString[] = {
	0xe3, 0xb0, 0xc4, 0x42, 0x98, 0xfc, 0x1c, 0x14, 0x9a, 0xfb, 0xf4, 0xc8, 0x99, 0x6f, 0xb9, 0x24,
	0x27, 0xae, 0x41, 0xe4, 0x64, 0x9b, 0x93, 0x4c, 0xa4, 0x95, 0x99, 0x1b, 0x78, 0x52, 0xb8, 0x55
};

const char sha384_emptyString[] = {
	0x38, 0xb0, 0x60, 0xa7, 0x51, 0xac, 0x96, 0x38, 0x4c, 0xd9, 0x32, 0x7e, 0xb1, 0xb1, 0xe3, 0x6a,
	0x21, 0xfd, 0xb7, 0x11, 0x14, 0xbe, 0x07, 0x43, 0x4c, 0x0c, 0xc7, 0xbf, 0x63, 0xf6, 0xe1, 0xda,
	0x27, 0x4e, 0xde, 0xbf, 0xe7, 0x6f, 0x65, 0xfb, 0xd5, 0x1a, 0xd2, 0xf1, 0x48, 0x98, 0xb9, 0x5b
};

const char sm3_emptyString[] = {
	0x1A, 0xB2, 0x1D, 0x83, 0x55, 0xCF, 0xA1, 0x7F, 0x8E, 0x61, 0x19, 0x48, 0x31, 0xE8, 0x1A, 0x8F,
	0x22, 0xBE, 0xC8, 0xC7, 0x28, 0xFE, 0xFB, 0x74, 0x7E, 0xD0, 0x35, 0xEB, 0x50, 0x82, 0xAA, 0x2B
};

static BIGNUM * bn_256 = NULL;
int ecc_api_init()
{
	int i;
	for (i = 0; i < ARRAYSIZE(_config); i++){
		_curves[i] = EC_GROUP_new_by_curve_name(_config[i].nid);
	}
	bn_256 = BN_new(); BN_set_word(bn_256, 256);
	return 0;
}

int ecc_api_done()
{
	int i;
	for (i = 0; i < ARRAYSIZE(_config); i++){
		EC_GROUP_free(_curves[i]);
	}
	BN_free(bn_256);
	return 0;
}

const char* ecc_curve_name(ecc_curve_id curve)
{
	if (curve < ARRAYSIZE(_config)) {
		return _config[curve].name;
	}
	return "unknown curve id";
}

static EC_KEY * EC_KEY_new_with_group(const EC_GROUP * g)
{
	EC_KEY * key = EC_KEY_new();
	if (key){
		EC_KEY_set_group(key, g);
		EC_KEY_set_asn1_flag(key, OPENSSL_EC_NAMED_CURVE);
	}
	return key;
}

static EC_KEY * EC_KEY_generate_with_group(const EC_GROUP * g)
{
	EC_KEY * key = EC_KEY_new();
	if (key){
		EC_KEY_set_group(key, g);
		EC_KEY_set_asn1_flag(key, OPENSSL_EC_NAMED_CURVE);
		if (!EC_KEY_generate_key(key)){
			ERR_print_errors_fp(stderr);
			fflush(stderr);
			EC_KEY_free(key);
			key = NULL;
		}
	}
	return key;
}

void * ecc_key_gen(ecc_curve_id pk_alg)
{
	return EC_KEY_generate_with_group(_curves[pk_alg]);
}

void * ecc_key_init(ecc_curve_id pk_alg, const char* priv_buf)
{
	EC_KEY * eckey = NULL;
	BIGNUM * bn = BN_new();
	const EC_GROUP * group = _curves[pk_alg];
	int fsize = (EC_GROUP_get_degree(group) + 7) / 8;
	if (group){
		if (BN_bin2bn((const unsigned char*)priv_buf, fsize, bn)){
			eckey = EC_KEY_new();
			if (eckey){
				if (EC_KEY_set_group(eckey, group)){
					if (EC_KEY_set_private_key(eckey, bn)){
						EC_POINT * point;
						point = EC_POINT_new(group);
						if (EC_POINT_mul(group, point, bn, NULL, NULL, NULL)){
							EC_KEY_set_public_key(eckey, point);
						}
						EC_POINT_free(point);
					}
				}
			}
		}
	}
	BN_free(bn);
	return (void*)eckey;
}

void   ecc_key_free(void* key)
{
	EC_KEY_free(key);
}

int ecc_key_private(void* key, char* buf)
{
	int len = -1;
	if (key){
		const BIGNUM   * bn;
		bn = EC_KEY_get0_private_key(key);
		if (bn){
			len = BN_num_bytes(bn);
			if (buf){
				BN_bn2bin(bn, (unsigned char*)buf);
			}
		}
	}
	return len;
}

static int ecc_fill_point(const EC_GROUP * g, const EC_POINT * p, char * px, char * py, int * psign)
{
	BIGNUM *x, *y;
	int fsize = -1;
	//fill public key data
	x = BN_new();
	y = BN_new();
	if (EC_POINT_get_affine_coordinates_GFp(g, p, x, y, NULL)){
		fsize = (EC_GROUP_get_degree(g) + 7) / 8;
		int bcount = BN_num_bytes(x);
		for(; bcount < fsize; bcount++)
			*(px++) = 0; // add padding with zeros
		BN_bn2bin(x, (unsigned char*)px);

		bcount = BN_num_bytes(y);
		for(; bcount < fsize; bcount++)
			*(py++) = 0; // add padding with zeros
		BN_bn2bin(y, (unsigned char*)py);
		if (psign) * psign = BN_is_odd(y);
	}
	BN_clear_free(x); BN_clear_free(y);
	return fsize;
}

int    ecc_key_public(void* key, char * px, char * py, int * psign)
{
	const EC_GROUP * ecgroup;
	const EC_POINT * ecpoint;
	const EC_KEY   * eckey = (EC_KEY*)key;
	if( key && px && py ) {
		ecgroup = EC_KEY_get0_group(eckey);
		ecpoint = EC_KEY_get0_public_key(eckey);
		if(ecgroup && ecpoint){
			return ecc_fill_point(ecgroup, ecpoint, px, py, psign);
		}
	}
	return -1;
}

static int _pass_cb(char *buf, int size, int rwflag, void *u)
{
	fprintf(stderr, "Ask for a pass phrase");
	return 0;
}

int    ecc_key_private_save(void* key, const char* path, ecc_format format)
{
	int rc = -1;
	EC_KEY   * eckey = (EC_KEY *)key;
	if (eckey){
		FILE * f = fopen(path, "wb");
		if (f){
			if (format == ecc_pem){
				rc = PEM_write_ECPrivateKey(f, eckey, NULL, NULL, 0, _pass_cb, NULL) ? 0 : -1;
				if (rc < 0){
					ERR_print_errors_fp(stderr);
				}
			}
			else{
				const EC_GROUP * ecgroup = EC_KEY_get0_group(eckey);
				int fsize = (EC_GROUP_get_degree(ecgroup) + 7) / 8;
				const BIGNUM   * ecbn;
				ecbn = EC_KEY_get0_private_key(eckey);
				if (ecbn){
					int bnlen = BN_num_bytes(ecbn);
					size_t len = (bnlen < fsize) ? fsize : bnlen;
					char * buf = (char *)OPENSSL_malloc(len*2+1);
					if (bnlen < len) memset(buf, 0, len - bnlen);
					BN_bn2bin(ecbn, (unsigned char *)(buf + len - bnlen));
					if (format == ecc_hex){
						char * c = _bin2hex(buf, len * 2 + 1, buf, len);
						*c = 0;
						len = c - buf;
						rc = 0;
					}
					rc = (len == fwrite(buf, 1, len, f)) ? 0 : -1;
					OPENSSL_free(buf);
				}
			}
			fclose(f);
			if (rc < 0){
				ERR_print_errors_fp(stderr);
				remove(path);
				rc = -1;
			}
		}
		else{
			perror(path);
		}
	}
	return rc;
}

void * ecc_key_private_load(const char* path, ecc_curve_id pk_alg)
{
	EC_KEY * eckey = NULL;
	FILE * f = fopen(path, "rb");
	if (f){
		eckey = PEM_read_ECPrivateKey(f, NULL, NULL, NULL);
		if (eckey == NULL){
			BIGNUM * bn = NULL;
			fseek(f, 0, SEEK_END);
			int len = ftell(f);
			fseek(f, 0, SEEK_SET);
			char * buf = OPENSSL_malloc(len + 1);
			if (len == fread(buf, 1, len, f)){
				buf[len] = 0;
				// try hex first
				if (len != BN_hex2bn(&bn, buf)){
					if (bn){
						BN_free(bn); bn = NULL;
					}
					bn = BN_bin2bn((const unsigned char*)buf, len, NULL);
				}
			}
			OPENSSL_free(buf);
			if (bn){
				eckey = EC_KEY_new();
				if (eckey){
					EC_KEY_set_group(eckey, _curves[pk_alg]);
					if (EC_KEY_set_private_key(eckey, bn)){
						EC_POINT * point;
						const EC_GROUP * group;
						group = EC_KEY_get0_group(eckey);
						point = EC_POINT_new(group);
						if (EC_POINT_mul(group, point, bn, NULL, NULL, NULL)){
							EC_KEY_set_public_key(eckey, point);
						}
						EC_POINT_free(point);
					}
					else{
						EC_KEY_free(eckey); eckey = NULL;
					}
				}
				BN_free(bn);
			}
		}
		fclose(f);
	}
	return eckey;
}

int ecc_key_public_save(void* key, const char* path, ecc_format format)
{
	EC_KEY   * eckey = (EC_KEY *)key;
	int rc = -1;
	if (eckey){
		FILE * f = fopen(path, "wb");
		if (f){
			if (format == ecc_pem){
				rc = PEM_write_EC_PUBKEY(f, eckey) ? 0 : -1;
			}
			else{
				size_t len;
				char * buf = NULL;
				const EC_POINT * point = EC_KEY_get0_public_key(eckey);
				const EC_GROUP * group = EC_KEY_get0_group(eckey);

				if (format == ecc_hex){
					buf = EC_POINT_point2hex(group, point, POINT_CONVERSION_UNCOMPRESSED, NULL);
					len = strlen(buf);
				}
				else if (format == ecc_bin){
					len = EC_POINT_point2oct(group, point, POINT_CONVERSION_UNCOMPRESSED, NULL, 0, NULL);
					if (len > 0){
						buf = OPENSSL_malloc(len + 1);
						if (len != EC_POINT_point2oct(group, point, POINT_CONVERSION_UNCOMPRESSED, (unsigned char*)buf, len, NULL)){
							OPENSSL_free(buf); buf = NULL;
						}
					}
				}
				if (buf){
					if (len == fwrite(buf, 1, len, f)){
						rc = 0;
					}
					OPENSSL_free(buf); buf = NULL;
				}
			}
			fclose(f);
			if (rc < 0){
				ERR_print_errors_fp(stderr);
				remove(path);
			}
		}
		else{
			perror(path);
		}
	}
	return rc;
}

void * ecc_key_public_set(ecc_curve_id pk_alg, ecc_point_type ptype, const char * px, const char * py)
{
	EC_KEY * eckey = NULL;
	EC_POINT * point = NULL;
	const EC_GROUP * group;
	BIGNUM *x, *y = NULL;
	int rc = 0;
	int fsize;

	group = _curves[pk_alg];
	point = EC_POINT_new(group);
	fsize = (EC_GROUP_get_degree(group) + 7) / 8;
	x = BN_bin2bn((const unsigned char*)px, fsize, NULL);
	if (x){
		if (ptype == ecc_uncompressed){
			y = BN_bin2bn((const unsigned char*)py, fsize, NULL);
			if (y){
				rc = EC_POINT_set_affine_coordinates_GFp(group, point, x, y, NULL);
				BN_clear_free(y);
			}
		}
		else {
			rc = EC_POINT_set_compressed_coordinates_GFp(group, point, x, ptype & 1, NULL);
		}
		BN_clear_free(x);
		if (rc){
			eckey = EC_KEY_new();
			if (eckey){
				EC_KEY_set_group(eckey, group);
				EC_KEY_set_public_key(eckey, point);
			}
		}
	}
	EC_POINT_free(point);
	return eckey;
}

void * ecc_key_public_load(const char* path, ecc_curve_id pk_alg)
{
	EC_KEY * eckey = NULL;
	FILE * f = fopen(path, "rb");
	EC_POINT * point = NULL;
	const EC_GROUP * group = _curves[pk_alg];
	if (f){
		eckey = PEM_read_EC_PUBKEY(f, &eckey, NULL, NULL);
		if (eckey == NULL){
			fseek(f, 0, SEEK_END);
			size_t len = ftell(f);
			fseek(f, 0, SEEK_SET);
			char * buf = OPENSSL_malloc(len + 1);
			if (len == fread(buf, 1, len, f)){
				buf[len] = 0;
				// try hex first
				point = EC_POINT_hex2point(group, buf, NULL, NULL);
				if (point == NULL){
					// try oct
					point = EC_POINT_new(group);
					if (!EC_POINT_oct2point(group, point, (const unsigned char*)buf, len, NULL)){
						EC_POINT_free(point);
						point = NULL;
					}
				}
				if (point){
					eckey = EC_KEY_new();
					if (eckey){
						EC_KEY_set_group(eckey, group);
						EC_KEY_set_public_key(eckey, point);
					}
					EC_POINT_free(point);
				}
			}
			OPENSSL_free(buf);
		}
		fclose(f);
	}
	return eckey;
}

int sha256_calculate(char* hash, const char * ptr, size_t len)
{
	SHA256_CTX ctx;
	SHA256_Init(&ctx);
	SHA256_Update(&ctx, ptr, len);
	SHA256_Final((unsigned char*)hash, &ctx);
	return 0;
}

int sha384_calculate(char* hash, const char * ptr, size_t len)
{
	SHA512_CTX ctx;
	SHA384_Init(&ctx);
	SHA384_Update(&ctx, ptr, len);
	SHA384_Final((unsigned char*)hash, &ctx);
	return 0;
}

int sm3_calculate(char* hash, const char * ptr, size_t len)
{
	SHA256_CTX ctx;
	SHA256_Init(&ctx);
	SHA256_Update(&ctx, ptr, len);
	SHA256_Final((unsigned char*)hash, &ctx);
	return 0;
}

int    ecc_sign(void * key, const char * hash, int hlength, char *r, char *s)
{
	EC_KEY   * eckey;

	eckey = EC_KEY_dup((EC_KEY*)key);
	if(eckey){
		ECDSA_SIG * ecdsa;
		int fsize = (EC_GROUP_get_degree(EC_KEY_get0_group(eckey)) + 7) / 8;
		ecdsa = ECDSA_do_sign(hash, hlength, eckey);
		EC_KEY_free(eckey);
		if (ecdsa){
			int i, bcount;
			const BIGNUM* sr;
			const BIGNUM* ss;
# if defined(OPENSSL_VERSION_NUMBER) && OPENSSL_VERSION_NUMBER >= 0x10100000L
			ECDSA_SIG_get0(ecdsa, &sr, &ss);
#else
			sr = ecdsa->r;
			ss = ecdsa->s;
#endif
			bcount = BN_num_bytes(sr);
			for(i=bcount; i < fsize; i++)
				*(r++) = 0; // add padding with zeros
			BN_bn2bin(sr, r);
			bcount = BN_num_bytes(ss);
			for(i=bcount; i < fsize; i++)
				*(s++) = 0; // add padding with zeros
			BN_bn2bin(ss, s);
			ECDSA_SIG_free(ecdsa);
			return 0;
		}
	}
	return -1;
}

int    ecc_verify(void * key, const char * hash, int hlength, const char *r, const char *s)
{
	int rc = -1;
	EC_KEY   * eckey;
	ECDSA_SIG * ecdsa;
	eckey = EC_KEY_dup((EC_KEY*)key);
	if (eckey){
		BIGNUM* sr;
		BIGNUM* ss;
		int fsize = (EC_GROUP_get_degree(EC_KEY_get0_group(eckey)) + 7) / 8;
		ecdsa = ECDSA_SIG_new();
		sr = BN_new(); ss = BN_new();
		BN_bin2bn(r, fsize, sr);
		BN_bin2bn(s, fsize, ss);
#if defined(OPENSSL_VERSION_NUMBER) && OPENSSL_VERSION_NUMBER >= 0x10100000L
		ECDSA_SIG_set0(ecdsa, sr, ss);
#else
        ecdsa->r = sr;
        ecdsa->s = ss;
#endif
		rc = ECDSA_do_verify(hash, hlength, ecdsa, eckey);
		ECDSA_SIG_free(ecdsa);
		BN_free(sr); BN_free(ss);
	}
	return rc;
}

/**
 * @brief Calculate Reconstruction Value for implicit certificate
 * Create public and private key at the same time
 * 
 * @param req_key [in]  EC_KEY containing request key
 * @param ca_key  [in]  EC_KEY containing CA private key
 * @param px      [in]  X coordinate of reconstruction value
 * @param py      [in]  Y coordinate of reconstruction value
 * @param psign   [in]  0/1 sign of the Y coordinate
 * @return        EC_KEY - end entity private/public key pair
 */
void * ecc_recvalue(void * req, char * px, char * py, int * psign)
{
	const EC_GROUP * g  = EC_KEY_get0_group(req);
	if( !g ){
		return NULL;
	}

	// generate eph key
	EC_KEY * eph = EC_KEY_generate_with_group(g);
	if (!eph){
		return NULL;
	}

	// generate public reconstruction value
	EC_POINT * Q = EC_POINT_new(g);
	EC_POINT_add(g, Q, EC_KEY_get0_public_key(eph), EC_KEY_get0_public_key(req), NULL);
	ecc_fill_point(g, Q, px, py, psign);
	EC_POINT_free(Q);

	return eph;
}

void * ecc_reconstruct(void * req, void * eph, void * ca, unsigned char * H)
{
	const EC_GROUP * g  = EC_KEY_get0_group(req);
	if( !g ){
		return NULL;
	}
	const BIGNUM * order = EC_GROUP_get0_order(g);
	BN_CTX * ctx = BN_CTX_new();
    BN_CTX_start(ctx);


	int fsize = (EC_GROUP_get_degree(g) + 7) / 8;
	BIGNUM * h = BN_CTX_get(ctx);
	BN_bin2bn(H, fsize, h);
	BN_rshift1(h, h);
	
    // Private reconstruction value
    //  r = (H * kEph + dCA) % order
    BIGNUM * r = BN_CTX_get(ctx);
    BIGNUM * b1 = BN_CTX_get(ctx);
    BN_mul(b1, EC_KEY_get0_private_key(eph), h, ctx);
    BN_mod_add(r, b1, EC_KEY_get0_private_key(ca), order, ctx);

    // Private key reconstruction
    // dU = (H * kReq + r) % order
    BIGNUM * dU = BN_CTX_get(ctx);
    BN_mul(b1, h, EC_KEY_get0_private_key(req), ctx);
    BN_mod_add(dU, b1, r, order, ctx);

    // Public key reconstruction
    // QU = H * Prv + QCA
    EC_POINT * p = EC_POINT_new(g);
    EC_POINT_mul(g, p, dU, NULL, NULL, ctx);

	EC_KEY * ukey = EC_KEY_new_with_group(g);
	EC_KEY_set_private_key(ukey, dU);
	EC_KEY_set_public_key(ukey, p);
	EC_POINT_free(p);

	BN_CTX_end(ctx);
	BN_CTX_free(ctx);

	return ukey;
}

static const char* _hexDigits = "0123456789ABCDEF";
char * _bin2hex(char * hex, size_t hlen, const char * bin, size_t blen)
{
	const unsigned char *b, *e;
	char * s;

	// sanity check
	if (hlen >= 0 && hlen < blen * 2) return NULL;

	b = (const unsigned char *)bin;
	e = b + blen - 1;
	s = hex + blen * 2;
	if (s < hex + hlen) *s = 0;
	for (; b <= e; e--){
		*(--s) = _hexDigits[(*e) & 0xF];
		*(--s) = _hexDigits[(*e) >> 4];
	}
	return hex + blen * 2;
}

