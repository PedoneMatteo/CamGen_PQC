#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <stdio.h>
#include <math.h>
#include "../cshared/copts.h"
#include "../cshared/cstr.h"
#include "ecc_api.h"
#include <ctype.h>
#include "mkgmtime.h"

#include "asn_application.h"
#include "EtsiTs103097Certificate.h"
#include "CountryOnly.h"
#include "xer_support.h"

#include "../../../liboqs/build/include/oqs/sig_dilithium.h"

#define CERT_MAX_SIZE 0x10000

static const char *_outPath = ".";
static const char *_searchPath = NULL;
static const char *_certName = NULL;
static char *_profileName = NULL;
static char *_signerName = NULL;
static ecc_format _outKeyFormat = ecc_bin;
static const char *_keyPath = NULL;
static int _force = 0;
static int _no_gen = 0;
static const char *_cfgFile = NULL;
static int _debug = 0;
static int _xer = 0;
static int _view = 0;

EtsiTs103097Certificate_t *_cert = NULL;
EtsiTs103097Certificate_t *_issuer = NULL;

int _tbsHashType = 0;
char _tbsHash[512]; // has space for issuer hash + signer hash
int _tbsHashLength = 0;

char _signerHashBuf[256]; // has space for issuer hash
const char *_signerHash = &_signerHashBuf[0];
int _signerHashLength = 0;

static const char *const _key_formats[] = {
	"bin", "hex", "pem", NULL};

typedef struct
{
	const char *cert;
	const char *vkey;
	const char *vkey_pub;
	const char *ekey;
	const char *ekey_pub;
} extensions_t;

#define EXT_CERT ".oer"
#define EXT_VKEY ".vkey"
#define EXT_EKEY ".ekey"
#define EXT_PUB "_pub"

static copt_t _options[] = {
	{"h?", "help", COPT_HELP, NULL, "Print this help page"},
	{"C", "config", COPT_CFGFILE, (void *)&_cfgFile, "Config file path [no cfg file]"},
	{"o", "out", COPT_STR, (void *)&_outPath, "Output path [current dir by default]"},
	{"k", "key-format", COPT_STRENUM, (void *)_key_formats, "Keys output format (bin|hex|pem)[binary by default]"},
	{"S", "certs", COPT_STR, (void *)&_searchPath, "Certificates search path [Output path by default]"},
	{"K", "keys", COPT_STR, (void *)&_keyPath, "Private key storage path [Output path by default]"},
	{"f", "force", COPT_BOOL, (void *)&_force, "Force regenerate existing certificate and keys. [use existing by default]"},
	{"e", "no-generate", COPT_BOOL, (void *)&_no_gen, "Do not generate keys. [generate if necessary by default]"},
	{"n", "name", COPT_STR, (void *)&_certName, "Certificate name (take from profile by default)"},
	{"s", "signer", COPT_STR, (void *)&_signerName, "Signer certificate name [take from profile by default]"},
	{"D", "debug", COPT_BOOL, (void *)&_debug, "Dump hashes and other values [false]"},
	{"v", "view", COPT_BOOL, (void *)&_view, "View OER certificate [false]"},
	{"x", "oxer", COPT_BOOL, (void *)&_xer, "Decode OER certificate to XER [false]"},
	{NULL, NULL, COPT_END, NULL, NULL}};

char *_bin2hex(char *hex, size_t hlen, const char *bin, size_t blen);

static int is_CurvePoint_empty(EccP256CurvePoint_t *point);

static int fill_Dilithium_keyPair(DilithiumKey_t *dilithium, int algorithmVersion, char *keyPath);
static int fill_curve_point_eccP256(EccP256CurvePoint_t *point, ecc_curve_id curveType, char *keyPath);
static int fill_curve_point_eccP384(EccP384CurvePoint_t *point, ecc_curve_id curveType, char *keyPath);
static void *fill_reconstruction_value(EccP256CurvePoint_t *point, void *r_key);
static void process_reconstruction_value(void *r_key, void *eph, void *ca, unsigned char *H, char *keyPath);

static int _issuer_parser_cb(pxml_chunk_type_e _type,
							 const void *_chunk_data, size_t _chunk_size, void *_key)
{
	char *f = cstrnstr((const char *)_chunk_data, _chunk_size, "name=\"");
	if (f)
	{
		_signerName = f + 6;
		f = cstrchr(_signerName, '"');
		_signerName = cstrndup(_signerName, f - _signerName);
	}
	return -1;
}

static asn_dec_rval_t IssuerIdentifier_xer_decoder(const asn_codec_ctx_t *opt_codec_ctx,
												   const asn_TYPE_descriptor_t *td, void **struct_ptr,
												   const char *opt_mname, const void *buf_ptr, size_t size)
{
	if (_signerName == NULL)
	{
		int stateContext = 0;
		pxml_parse(&stateContext, buf_ptr, size, _issuer_parser_cb, NULL);
	}
	return CHOICE_decode_xer(opt_codec_ctx, td, struct_ptr, opt_mname, buf_ptr, size);
}

static asn_dec_rval_t CountryOnly_xer_decoder(const asn_codec_ctx_t *opt_codec_ctx,
											  const asn_TYPE_descriptor_t *td, void **struct_ptr,
											  const char *opt_mname, const void *buf_ptr, size_t size)
{
	// for the future to support letter codes
	return NativeInteger_decode_xer(opt_codec_ctx, td, struct_ptr, opt_mname, buf_ptr, size);
}

typedef struct overrun_encoder_key
{
	void *buffer;
	size_t buffer_size;
	size_t computed_size;
} overrun_encoder_key;

static asn_enc_rval_t ToBeSignedCertificate_oer_encoder(const asn_TYPE_descriptor_t *td,
														const asn_oer_constraints_t *constraints, const void *sptr,
														asn_app_consume_bytes_f *cb, void *app_key)
{
	asn_enc_rval_t rc;
	overrun_encoder_key *a = (overrun_encoder_key *)app_key;
	const char *oer = ((const char *)a->buffer) + a->computed_size;
	rc = SEQUENCE_encode_oer(td, constraints, sptr, cb, app_key);
	if (rc.encoded > 0)
	{
		if (_debug)
		{
			char hex[257 * 2];
			*_bin2hex(hex, sizeof(hex) - 1, oer, ((rc.encoded > 256) ? 256 : rc.encoded)) = 0;
			fprintf(stderr, "DEBUG: ToBeSigned OER[%zu]=%s\n", rc.encoded, hex);
		}
		// calculate hash
		if (_signerHashLength == sha384_hash_size)
		{
			_tbsHashLength = 48;
			sha384_calculate(_tbsHash, (const char *)oer, rc.encoded);
		}
		else
		{
			_tbsHashLength = 32;
			sha256_calculate(_tbsHash, (const char *)oer, rc.encoded);
		}
	}
	return rc;
}

static ecc_curve_id _pk_type_to_curveid[] = {
	0,
	ecies_nistp256,		  // Signature_PR_ecdsaNistP256Signature,
	ecies_brainpoolp256r, // Signature_PR_ecdsaBrainpoolP256r1Signature,
	ecies_brainpoolp384r, // Signature_PR_ecdsaBrainpoolP384r1Signature,
	ecies_nistp384,		  // Signature_PR_ecdsaNistP384Signature
};

static const char _unsupported[] = "unsupported";
static const char *_pk_curve_names[] = {
	"nistp256",
	"brainpoolp256r",
	"brainpoolp384r",
	"nistp384",
	"sm2",
	&_unsupported[0],
	&_unsupported[0],
	&_unsupported[0]};

static ecc_curve_id _pk_type_to_hashid[] = {
	0,
	sha_256, // Signature_PR_ecdsaNistP256Signature,
	sha_256, // Signature_PR_ecdsaBrainpoolP256r1Signature,
	sha_384, // Signature_PR_ecdsaBrainpoolP384r1Signature
	sha_384, // Signature_PR_ecdsaNistP384Signature,
	sm3		 // Signature_PR_sm2Signature
};

static size_t _pk_type_to_hashsize[] = {
	0,
	sha256_hash_size, // Signature_PR_ecdsaNistP256Signature,
	sha256_hash_size, // Signature_PR_ecdsaBrainpoolP256r1Signature,
	sha384_hash_size, // Signature_PR_ecdsaBrainpoolP384r1Signature
	sha384_hash_size, // Signature_PR_ecdsaNistP384Signature,
	sm3_hash_size	  // Signature_PR_sm2Signature
};

static void *search_private_Dilithium_key(const char *sName, int alg)
{
	char *secretKey = malloc(OQS_SIG_dilithium_2_length_secret_key);
	char *path = cvstrdup(_keyPath, "/", sName, EXT_VKEY, NULL);
	FILE *f = fopen(path, "rb");

	if (f == NULL)
	{
		fprintf(stderr, "Error: impossible to open the file%s\n", path);
		free(secretKey);
		free(path);
		return NULL;
	}

	// read content of file
	size_t bytesRead = fread(secretKey, 1, OQS_SIG_dilithium_2_length_secret_key, f);

	if (bytesRead != OQS_SIG_dilithium_2_length_secret_key)
	{
		fprintf(stderr, "Error: wrong len of bytes read %s\n", path);
		free(secretKey);
		secretKey = NULL;
	}

	fclose(f);
	free(path);

	return secretKey;
}

static void *search_private_key(const char *sName, ecc_curve_id alg)
{
	char *pk = cvstrdup(_keyPath, "/", sName, EXT_VKEY, NULL);

	void *k = ecc_key_private_load(pk, alg);
	if (k == NULL)
	{
		free(pk);
		pk = cvstrdup(_searchPath, "/", sName, EXT_VKEY, NULL);
		if (k == NULL)
		{
			fprintf(stderr, "%s: Private key not found\n", pk);
		}
	}
	free(pk);
	return k;
}

static asn_enc_rval_t Signature_oer_encoder(const asn_TYPE_descriptor_t *td,
											const asn_oer_constraints_t *constraints, const void *sptr,
											asn_app_consume_bytes_f *cb, void *app_key)
{
	Signature_t *s = (Signature_t *)sptr;

	if (is_CurvePoint_empty(&s->choice.ecdsaNistP256Signature.rSig) && _tbsHashType < 6)
	{
		printf("\t\n - - - Signature oer encoder - - - \n");
		// look for signer private key
		ecc_curve_id alg = _pk_type_to_curveid[s->present];
		printf("alg = %ld\n", alg);
		ecc_hash_id hashId = _pk_type_to_hashid[s->present];
		printf("hashId = %ld\n", hashId);

		const char *sName = _signerName;
		printf("signerName = %s\n", _signerName);
		printf(" issuer present? = %d\n", _cert->issuer.present);

		if (sName == NULL && _cert->issuer.present == IssuerIdentifier_PR_self)
			sName = _certName;

		void *k = search_private_key(sName, alg);
		if (k == NULL)
		{
			ASN__ENCODE_FAILED;
		}

		char h[48];
		int hl = 0;
		// calculate joint hash
		memcpy(_tbsHash + _tbsHashLength, _signerHash, _signerHashLength);
		switch (hashId)
		{
		case sha_256:
			sha256_calculate(h, _tbsHash, _tbsHashLength + _signerHashLength);
			hl = sha256_hash_size;
			break;
		case sha_384:
			sha384_calculate(h, _tbsHash, _tbsHashLength + _signerHashLength);
			hl = sha384_hash_size;
			break;
		case sm3:
			sm3_calculate(h, _tbsHash, _tbsHashLength + _signerHashLength);
			hl = sm3_hash_size;
			break;
		}

		if (_debug)
		{
			char hex[48 * 3 + 1];
			*_bin2hex(hex, sizeof(hex), _tbsHash, _tbsHashLength) = 0;
			fprintf(stderr, "DEBUG: ToBeSignedHash[%d]=%s\n", _tbsHashLength, hex);
			*_bin2hex(hex, sizeof(hex), _signerHash, _signerHashLength) = 0;
			fprintf(stderr, "DEBUG: SignerHash[%d]=%s\n", _signerHashLength, hex);
			*_bin2hex(hex, sizeof(hex), h, hl) = 0;
			fprintf(stderr, "DEBUG: JoinedHash[%d]=%s\n", hl, hex);
		}

		// initialize buffers
		OCTET_STRING_fromBuf(&s->choice.ecdsaNistP256Signature.rSig.choice.x_only, h, hl);
		OCTET_STRING_fromBuf(&s->choice.ecdsaNistP256Signature.sSig, h, hl);

		ecc_sign(k, h, hl, s->choice.ecdsaNistP256Signature.rSig.choice.x_only.buf, s->choice.ecdsaNistP256Signature.sSig.buf);
	}
	else
	{ // added Dilithium Signature
		printf("\t\n - - - Signature oer encoder - - - \n");
		// look for signer private key
		ecc_hash_id hashId = _pk_type_to_hashid[s->present];
		printf("\nhashId = %ld\n", hashId);

		const char *sName = _signerName;
		printf("signerName = %s\n", _signerName);
		printf(" issuer present? = %d\n", _cert->issuer.present);

		if (sName == NULL && _cert->issuer.present == IssuerIdentifier_PR_self)
			sName = _certName;
		printf("sName = %s\n", sName);

		uint8_t *secret_key = search_private_Dilithium_key(sName, 2);
		if (secret_key == NULL)
		{
			ASN__ENCODE_FAILED;
		}
		
		char h[48];
		int hl = 0;
		// calculate joint hash
		memcpy(_tbsHash + _tbsHashLength, _signerHash, _signerHashLength);
		switch (hashId)
		{
		case sha_256:
			sha256_calculate(h, _tbsHash, _tbsHashLength + _signerHashLength);
			hl = sha256_hash_size;
			break;
		case sha_384:
			sha384_calculate(h, _tbsHash, _tbsHashLength + _signerHashLength);
			hl = sha384_hash_size;
			break;
		case sm3:
			sm3_calculate(h, _tbsHash, _tbsHashLength + _signerHashLength);
			hl = sm3_hash_size;
			break;
		}

		if (_debug)
		{
			char hex[48 * 3 + 1];
			*_bin2hex(hex, sizeof(hex), _tbsHash, _tbsHashLength) = 0;
			fprintf(stderr, "DEBUG: ToBeSignedHash[%d]=%s\n", _tbsHashLength, hex);
			*_bin2hex(hex, sizeof(hex), _signerHash, _signerHashLength) = 0;
			fprintf(stderr, "DEBUG: SignerHash[%d]=%s\n", _signerHashLength, hex);
			*_bin2hex(hex, sizeof(hex), h, hl) = 0;
			fprintf(stderr, "DEBUG: JoinedHash[%d]=%s\n", hl, hex);
		}
		
		s->choice.dilithiumsignature.signature.buf = malloc(OQS_SIG_dilithium_2_length_signature);
		s->choice.dilithiumsignature.signature.size = OQS_SIG_dilithium_2_length_signature;

		OQS_STATUS returnedCheck = OQS_SIG_dilithium_2_sign(s->choice.dilithiumsignature.signature.buf, &(s->choice.dilithiumsignature.signature.size), h, hl, secret_key);
		if (returnedCheck != 0)
		{
			printf("\n Error: during gen Signature phase\n");
			return;
		}
		free(secret_key);
	}
	printf("\n%s\n",td->name);
	return asn_OP_CHOICE.oer_encoder(td, constraints, sptr, cb, app_key);
}

static asn_dec_rval_t PublicVerificationKey_oer_decoder(const struct asn_codec_ctx_s *opt_codec_ctx,
														const struct asn_TYPE_descriptor_s *td, const asn_oer_constraints_t *constraints,
														void **struct_ptr, const void *buf_ptr, size_t size)
{
	return asn_OP_CHOICE.oer_decoder(opt_codec_ctx, td, constraints, struct_ptr, buf_ptr, size);
}

static asn_dec_rval_t ServiceSpecificPermissions_oer_decoder(const struct asn_codec_ctx_s *opt_codec_ctx,
															 const struct asn_TYPE_descriptor_s *td, const asn_oer_constraints_t *constraints,
															 void **struct_ptr, const void *buf_ptr, size_t size)
{
	return asn_OP_CHOICE.oer_decoder(opt_codec_ctx, td, constraints, struct_ptr, buf_ptr, size);
}

static int Time32_print_struct(const asn_TYPE_descriptor_t *td, const void *sptr,
							   int ilevel, asn_app_consume_bytes_f *cb, void *app_key)
{
	int rc = NativeInteger_print(td, sptr, ilevel, cb, app_key);
	if (rc == 0)
	{
		const long *native = (const long *)sptr;
		if (native)
		{
			const char *s = stritsdate32(*native);
			if (s)
			{
				if (cb(" --(", 4, app_key) < 0)
					return -1;
				if (cb(s, strlen(s), app_key) < 0)
					return -1;
				if (cb(")", 1, app_key) < 0)
					return -1;
			}
		}
	}
	return rc;
}
int main(int argc, char **argv)
{
	// set default time to the begining of this year
	//_setup_default_time();

	// parse options
	argc = coptions(argc, argv, COPT_HELP_NOVALUES, _options);
	if (argc < 2)
	{
		if (argc < 0 && (((int)0) - argc) < ((sizeof(_options) / sizeof(_options[0])) - 1))
		{
			printf("Unknown option %s\n", argv[0 - argc]);
		}
		const char *a = strrchr(argv[0], '/');
		if (a == NULL)
			a = argv[0];
		coptions_help(stdout, a, COPT_HELP_NOVALUES, _options, "<profile> [signer]");
		return -1;
	}

	if (_searchPath == NULL)
		_searchPath = _outPath;
	if (_keyPath == NULL)
		_keyPath = _outPath;
	if (_xer)
		_view = 1;

	_outKeyFormat = copts_enum_value(_options, 3, _key_formats);

	if (argc > 2)
	{
		// set signer certificate file name
		_signerName = argv[2];
	}

	if (ecc_api_init())
	{
		fprintf(stderr, "Cannot initialize ECC engine\n");
		return -1;
	}

	_profileName = cstrdup(cstrlastpathelement(argv[1]));
	if (_profileName)
	{
		char *p = strrchr(_profileName, '.');
		if (p)
			*p = 0;
	}
	if (_certName == NULL)
	{
		_certName = _profileName;
	}
	printf("\n Cert Name = %s\n", _certName);

	// load XER file
	char *buf = malloc(CERT_MAX_SIZE);
	char *ebuf;
	EtsiTs103097Certificate_t *cert = NULL;
	printf("\n argv[1] = %s\n", argv[1]);

	ebuf = cstrnload(buf, CERT_MAX_SIZE, argv[1]); // argv[1] = CERT_IUT_A_RCA.xer
	if (ebuf == NULL)
	{
		fprintf(stderr, "%s: Certificate%s not found\n", argv[1], (_view) ? "" : " profile");
		return -1;
	}

	FILE *f;
	asn_dec_rval_t rc_d;
	asn_enc_rval_t rc_e;

	asn_TYPE_operation_t issuerOps = *asn_DEF_IssuerIdentifier.op;
	asn_DEF_IssuerIdentifier.op = &issuerOps;

	asn_TYPE_operation_t tbsOps = *asn_DEF_ToBeSignedCertificate.op;
	asn_DEF_ToBeSignedCertificate.op = &tbsOps;

	asn_TYPE_operation_t signatureOps = *asn_DEF_Signature.op;
	asn_DEF_Signature.op = &signatureOps;

	asn_TYPE_operation_t countryOnlyOps = *asn_DEF_CountryOnly.op;
	asn_DEF_CountryOnly.op = &countryOnlyOps;

	asn_TYPE_operation_t publicVerificationKeyOps = *asn_DEF_PublicVerificationKey.op;
	asn_DEF_PublicVerificationKey.op = &publicVerificationKeyOps;

	asn_TYPE_operation_t serviceSpecificPermissionsOps = *asn_DEF_ServiceSpecificPermissions.op;
	asn_DEF_ServiceSpecificPermissions.op = &serviceSpecificPermissionsOps;

	asn_TYPE_operation_t time32Ops = *asn_DEF_Time32.op;
	asn_DEF_Time32.op = &time32Ops;

	issuerOps.xer_decoder = IssuerIdentifier_xer_decoder;
	tbsOps.oer_encoder = ToBeSignedCertificate_oer_encoder;
	signatureOps.oer_encoder = Signature_oer_encoder;
	countryOnlyOps.xer_decoder = CountryOnly_xer_decoder;
	publicVerificationKeyOps.oer_decoder = PublicVerificationKey_oer_decoder;
	serviceSpecificPermissionsOps.oer_decoder = ServiceSpecificPermissions_oer_decoder;
	time32Ops.print_struct = Time32_print_struct;

	if (_view)
	{ // non entra per RCA, _view = 0
		char hash[50], hash_hex[256];
		size_t hLen;
		rc_d = asn_decode(NULL, ATS_BASIC_OER, &asn_DEF_EtsiTs103097Certificate, (void **)&cert, buf, ebuf - buf);
		if (rc_d.code != RC_OK)
		{
			fprintf(stderr, "%s: failed to load at position %d\n  %.30s\n", argv[1], (int)rc_d.consumed, buf + rc_d.consumed);
			return -1;
		}
		if (!_xer)
		{
			if (cert->toBeSigned.verifyKeyIndicator.present == VerificationKeyIndicator_PR_verificationKey &&
				cert->toBeSigned.verifyKeyIndicator.choice.verificationKey.present == PublicVerificationKey_PR_ecdsaBrainpoolP384r1)
			{
				hLen = 48;
				sha384_calculate(hash, (const char *)buf, rc_d.consumed);
			}
			else
			{
				hLen = 32;
				sha256_calculate(hash, (const char *)buf, rc_d.consumed);
			}
			*_bin2hex(hash_hex, sizeof(hash_hex), hash, hLen) = 0;
		}

		rc_e = asn_encode_to_buffer(NULL, _xer ? ATS_CANONICAL_XER : ATS_NONSTANDARD_PLAINTEXT, &asn_DEF_EtsiTs103097Certificate, cert, buf, CERT_MAX_SIZE);
		if (rc_e.encoded < 0)
		{
			fprintf(stderr, "%s: %s encoding failed for %s\n", argv[1], _xer ? "XER" : "text", rc_e.failed_type->name);
			return -1;
		}
		if (!_xer)
		{
			printf("\nhash digest\n");
			fprintf(stderr, "Hash  : %s\n", hash_hex);
			fprintf(stderr, "Digest: %s\n", hash_hex + (hLen - 8) * 2);
		}
		fwrite(buf, 1, rc_e.encoded, stdout);
		return 0;
	}

	void *rv_req_key = NULL;
	void *rv_eph_key = NULL;

	PublicVerificationKey_PR hashType = PublicVerificationKey_PR_NOTHING;

	rc_d = asn_decode(NULL, ATS_BASIC_XER, &asn_DEF_EtsiTs103097Certificate, (void **)&cert, buf, ebuf - buf);
	if (rc_d.code != RC_OK)
	{
		fprintf(stderr, "%s: failed to load at position %d\n  %.30s\n", _profileName, (int)rc_d.consumed, buf + rc_d.consumed);
		return -1;
	}

	// check signer
	if (!_signerName && cert->issuer.present != IssuerIdentifier_PR_self)
	{
		fprintf(stderr, "%s: unknown signer\n", argv[1]);
		return -1;
	}
	if (_signerName)
	{
		printf("\n signer \n"); // non entra per RCA
		cvstrncpy(buf, CERT_MAX_SIZE, _searchPath, "/", _signerName, ".oer", NULL);
		ebuf = cstrnload(buf, CERT_MAX_SIZE, buf);
		if (ebuf == NULL)
		{
			fprintf(stderr, "%s: signer certificate not found", _signerName);
			return -1;
		}
		// decode it to detect hash algorythm
		asn_dec_rval_t rc_d;
		EtsiTs103097Certificate_t *signer = NULL;
		rc_d = asn_decode(NULL, ATS_BASIC_OER, &asn_DEF_EtsiTs103097Certificate, (void **)&signer, buf, ebuf - buf);
		if (rc_d.code != RC_OK)
		{
			fprintf(stderr, "%s: failed to load signer certificate at position %d\n  %.30s\n", _signerName, (int)rc_d.consumed, buf + rc_d.consumed);
			return -1;
		}
		switch (signer->toBeSigned.verifyKeyIndicator.present)
		{
		case VerificationKeyIndicator_PR_verificationKey:
			hashType = signer->toBeSigned.verifyKeyIndicator.choice.verificationKey.present;
			break;
		case VerificationKeyIndicator_PR_reconstructionValue:
			hashType = PublicVerificationKey_PR_ecdsaNistP256;
			break;
		case VerificationKeyIndicator_PR_NOTHING:
			fprintf(stderr, "%s: signer verification indicator type is unknown\n", _signerName);
			return -1;
		}
		_tbsHashType = hashType;

		if (signer->signature && signer->signature->choice.ecdsaNistP256Signature.rSig.present != EccP256CurvePoint_PR_x_only)
		{
			char *pTypePtr = buf + rc_d.consumed - 1 - (2 * _pk_type_to_hashsize[hashType]);
			*pTypePtr = 0x80;
		}

		switch (hashType)
		{
		case PublicVerificationKey_PR_ecdsaBrainpoolP256r1:
		case PublicVerificationKey_PR_ecdsaNistP256:
			if (cert->issuer.present == IssuerIdentifier_PR_NOTHING)
				cert->issuer.present = IssuerIdentifier_PR_sha256AndDigest;
			sha256_calculate(_signerHashBuf, buf, ebuf - buf);
			_signerHash = &_signerHashBuf[0];
			_signerHashLength = sha256_hash_size;
			OCTET_STRING_fromBuf(&cert->issuer.choice.sha256AndDigest, &_signerHash[sha256_hash_size - 8], 8);
			break;
		case PublicVerificationKey_PR_ecdsaBrainpoolP384r1:
		case PublicVerificationKey_PR_ecdsaNistP384:
			if (cert->issuer.present == IssuerIdentifier_PR_NOTHING)
				cert->issuer.present = IssuerIdentifier_PR_sha384AndDigest;
			sha384_calculate(_signerHashBuf, buf, ebuf - buf);
			_signerHash = &_signerHashBuf[0];
			_signerHashLength = sha384_hash_size;
			OCTET_STRING_fromBuf(&cert->issuer.choice.sha384AndDigest, &_signerHash[sha384_hash_size - 8], 8);
			break;
		case PublicVerificationKey_PR_dilithiumKey:
			printf("\n Dilithium\n");
			break;
		case PublicVerificationKey_PR_ecsigSm2:
		default:
			fprintf(stderr, "%s: signer verification key type curve is unknown\n", _signerName);
			return -1;
		}
		if (signer)
		{
			ASN_STRUCT_FREE(asn_DEF_EtsiTs103097Certificate, signer);
		}
	}
	else
	{
		printf("\n self signed\n");
		// self-signed
		// use hash(empty string)
		printf("\n cert->toBeSigned.verifyKeyIndicator.present = %d\n", cert->toBeSigned.verifyKeyIndicator.present); //-->1
		switch (cert->toBeSigned.verifyKeyIndicator.present)
		{
		case VerificationKeyIndicator_PR_verificationKey:
			hashType = cert->toBeSigned.verifyKeyIndicator.choice.verificationKey.present;
			printf("\n HASH TYPE = %d\n", hashType);
			if (hashType == 0)
				hashType = 6;
			printf("\n cert->toBeSigned.verifyKeyIndicator.choice.verificationKey.present = %d\n", hashType); // hashType = 1
			break;
		case VerificationKeyIndicator_PR_reconstructionValue:
			fprintf(stderr, "%s: self-signed certificate can not contain reconstruction value\n", _signerName);
			return -1;
		case VerificationKeyIndicator_PR_NOTHING:
			fprintf(stderr, "%s: signer verification indicator type is unknown\n", _signerName);
			return -1;
		}

		_tbsHashType = cert->toBeSigned.verifyKeyIndicator.choice.verificationKey.present = hashType;
		printf("\n hashtype = %d\n", hashType);
		switch (hashType)
		{
		case PublicVerificationKey_PR_ecdsaBrainpoolP256r1:
		case PublicVerificationKey_PR_ecdsaNistP256: // entra qui
			_signerHash = &sha256_emptyString[0];
			_signerHashLength = sha256_hash_size; // 32
			break;
		case PublicVerificationKey_PR_ecdsaBrainpoolP384r1:
		case PublicVerificationKey_PR_ecdsaNistP384:
			_signerHash = &sha384_emptyString[0];
			_signerHashLength = sha384_hash_size;
			break;
		case PublicVerificationKey_PR_ecsigSm2:
			_signerHash = &sm3_emptyString[0];
			_signerHashLength = sm3_hash_size;
			break;
		case PublicVerificationKey_PR_dilithiumKey:
			printf("\n Dilithium\n");
			_signerHash = &sha256_emptyString[0];
			_signerHashLength = sha256_hash_size; // 32
			break;
		default:
			fprintf(stderr, "Unknown verification key curve type\n");
			return -1;
		}
	}

	// generate keys if necessary
	// buf = name of private key file
	int rc = -1;

	/* after this call, buf contains "outputcertificates/CERT_IUT_A_RCA_Dilithium.vkey" */
	cvstrncpy(buf, CERT_MAX_SIZE, _keyPath, "/", _certName, EXT_VKEY, NULL);
	printf("\n cert->toBeSigned.verifyKeyIndicator.present = %d\n", cert->toBeSigned.verifyKeyIndicator.present);
	printf("\n cert->toBeSigned.verifyKeyIndicator.choice.verificationKey.present = %d\n", cert->toBeSigned.verifyKeyIndicator.choice.verificationKey.present);

	switch (cert->toBeSigned.verifyKeyIndicator.present)
	{
	case VerificationKeyIndicator_PR_verificationKey:
		switch (cert->toBeSigned.verifyKeyIndicator.choice.verificationKey.present)
		{
		case PublicVerificationKey_PR_ecdsaNistP256: // entra qui nella versione iniziale
			rc = fill_curve_point_eccP256(&cert->toBeSigned.verifyKeyIndicator.choice.verificationKey.choice.ecdsaNistP256, ecies_nistp256, buf);
			break;
		case PublicVerificationKey_PR_ecdsaBrainpoolP256r1:
			rc = fill_curve_point_eccP256(&cert->toBeSigned.verifyKeyIndicator.choice.verificationKey.choice.ecdsaBrainpoolP256r1, ecies_brainpoolp256r, buf);
			break;
		case PublicVerificationKey_PR_ecdsaBrainpoolP384r1:
			rc = fill_curve_point_eccP384(&cert->toBeSigned.verifyKeyIndicator.choice.verificationKey.choice.ecdsaBrainpoolP384r1, ecies_brainpoolp384r, buf);
			break;
		case PublicVerificationKey_PR_ecdsaNistP384:
			rc = fill_curve_point_eccP384(&cert->toBeSigned.verifyKeyIndicator.choice.verificationKey.choice.ecdsaNistP384, ecies_nistp384, buf);
			break;
		case PublicVerificationKey_PR_dilithiumKey: // ora entra qui
			rc = fill_Dilithium_keyPair(&cert->toBeSigned.verifyKeyIndicator.choice.verificationKey.choice.dilithiumKey, 2, buf);
			break;
		default:
			fprintf(stderr, "Unknown verification key curve type\n");
			return -1;
		}
		if (rc < 0)
		{
			fprintf(stderr, "%s: Cannot prepare verification key of type %s\n", buf,
					asn_MBR_PublicVerificationKey_1[cert->toBeSigned.verifyKeyIndicator.choice.verificationKey.present - 1].name);
			return -1;
		}
		break;
	case VerificationKeyIndicator_PR_reconstructionValue:
		rv_req_key = ecc_key_gen(ecies_nistp256);
		// look for CA key
		rv_eph_key = fill_reconstruction_value(&cert->toBeSigned.verifyKeyIndicator.choice.reconstructionValue, rv_req_key);
		if (rv_eph_key == NULL)
		{
			fprintf(stderr, "%s: Cannot calculate reconstruction value \n", buf);
			ecc_key_free(rv_req_key);
			rv_req_key = NULL;
			return -1;
		}
		rc = 0;
		break;
	case VerificationKeyIndicator_PR_NOTHING:
	default:
		fprintf(stderr, "%s: Unknown verification key indicator type %d\n", buf, cert->toBeSigned.verifyKeyIndicator.present - 1);
		break;
	}
	if (rc < 0)
	{
		return -1;
	}

	if (cert->toBeSigned.encryptionKey)
	{ // non entra per RCA
		printf(" cert->toBeSigned.encryptionKey->publicKey.present = %d\n", cert->toBeSigned.encryptionKey->publicKey.present);
		rc = -1;
		cvstrncpy(buf, CERT_MAX_SIZE, _keyPath, "/", _certName, EXT_EKEY, NULL);
		switch (cert->toBeSigned.encryptionKey->publicKey.present)
		{
		case BasePublicEncryptionKey_PR_NOTHING:
			cert->toBeSigned.encryptionKey->publicKey.present = BasePublicEncryptionKey_PR_eciesNistP256;
		case BasePublicEncryptionKey_PR_eciesNistP256:
			rc = fill_curve_point_eccP256(&cert->toBeSigned.encryptionKey->publicKey.choice.eciesNistP256, ecies_nistp256, buf);
			break;
		case BasePublicEncryptionKey_PR_eciesBrainpoolP256r1:
			rc = fill_curve_point_eccP256(&cert->toBeSigned.encryptionKey->publicKey.choice.eciesBrainpoolP256r1, ecies_brainpoolp256r, buf);
			break;
		default:
			fprintf(stderr, "Unknown encryption key curve type %d\n", cert->toBeSigned.encryptionKey->publicKey.present);
			break;
		}
		if (rc < 0)
		{
			fprintf(stderr, "%s: Cannot prepare encryption key\n", buf);
			return -1;
		}
	}
	/* after this call, buf contains "outputcertificates/CERT_IUT_A_RCA_Dilithium.oer" */
	cvstrncpy(buf, CERT_MAX_SIZE, _outPath, "/", _certName, EXT_CERT, NULL);

	f = fopen(buf, "wb");
	if (f == NULL)
	{
		perror(buf);
		return -1;
	}

	_cert = cert;
	// Encode as OER
	rc_e = asn_encode_to_buffer(NULL, ATS_CANONICAL_OER, &asn_DEF_EtsiTs103097Certificate, cert, buf, CERT_MAX_SIZE);
	if (rc_e.encoded < 0)
	{
		fprintf(stderr, "%s: OER encoding failed for %s\n", _certName, rc_e.failed_type->name);
		return -1;
	}
	fwrite(buf, 1, rc_e.encoded, f);
	fclose(f);

	if (rv_eph_key)
	{ // non entra per RCA
		const char *sName = _signerName;
		if (sName == NULL && _cert->issuer.present == IssuerIdentifier_PR_self)
			sName = _certName;

		void *ca = search_private_key(sName, ecies_nistp256);
		if (ca == NULL)
		{
			fprintf(stderr, "FATAL: Private key for %s not found\n", sName);
		}

		char h[48];
		int hl = 0;
		// calculate joint hash
		memcpy(_tbsHash + _tbsHashLength, _signerHash, _signerHashLength);
		sha256_calculate(h, _tbsHash, _tbsHashLength + _signerHashLength);
		hl = sha256_hash_size;

		if (_debug)
		{
			char hex[48 * 3 + 1];
			*_bin2hex(hex, sizeof(hex), _tbsHash, _tbsHashLength) = 0;
			fprintf(stderr, "DEBUG: ToBeSignedHash[%d]=%s\n", _tbsHashLength, hex);
			*_bin2hex(hex, sizeof(hex), _signerHash, _signerHashLength) = 0;
			fprintf(stderr, "DEBUG: SignerHash[%d]=%s\n", _signerHashLength, hex);
			*_bin2hex(hex, sizeof(hex), h, hl) = 0;
			fprintf(stderr, "DEBUG: JoinedHash[%d]=%s\n", hl, hex);
		}

		cvstrncpy(buf, CERT_MAX_SIZE, _keyPath, "/", _certName, NULL);
		process_reconstruction_value(rv_req_key, rv_eph_key, ca, h, buf);
		ecc_key_free(ca);
		ecc_key_free(rv_eph_key);
		ecc_key_free(rv_req_key);
	}

	if (cert)
	{
		ASN_STRUCT_FREE(asn_DEF_EtsiTs103097Certificate, cert);
	}
	return 0;
}

static int is_CurvePoint_empty(EccP256CurvePoint_t *point)
{
	switch (point->present)
	{
	case EccP256CurvePoint_PR_x_only:
	case EccP256CurvePoint_PR_compressed_y_0:
	case EccP256CurvePoint_PR_compressed_y_1:
		return point->choice.x_only.size < 32;
	case EccP256CurvePoint_PR_uncompressedP256:
		return point->choice.uncompressedP256.x.size < 32 || point->choice.uncompressedP256.y.size < 32;
	default:
		break;
	}
	return 1;
}

static void *gen_or_load_public_key(ecc_curve_id curveType, char *keyPath)
{
	void *key = NULL;
	char *e_pub = keyPath + strlen(keyPath);
	if (!_force)
	{
		// check for public key
		strcpy(e_pub, EXT_PUB);
		key = ecc_key_public_load(keyPath, curveType);
		if (_debug && key)
		{
			fprintf(stderr, "DEBUG: use pre-generated key %s\n", keyPath);
		}
		*e_pub = 0;
	}
	else
	{
		remove(keyPath);
	}
	if (key == NULL)
	{
		key = ecc_key_private_load(keyPath, curveType);
		if (key == NULL)
		{
			if (_no_gen)
			{
				fprintf(stderr, "ERROR: Key %s or %s%s not found\n", keyPath, keyPath, EXT_PUB);
				return NULL;
			}
			if (_debug)
			{
				fprintf(stderr, "DEBUG: generate key %s\n", keyPath);
			}
			key = ecc_key_gen(curveType);
			if (key == NULL)
			{
				fprintf(stderr, "ERROR: key %s generation failed for curve %s\n", keyPath, _pk_curve_names[curveType & 7]);
				return NULL;
			}
			ecc_key_private_save(key, keyPath, _outKeyFormat);
			strcpy(e_pub, EXT_PUB);
			ecc_key_public_save(key, keyPath, _outKeyFormat);
		}
		else
		{
			if (_debug)
			{
				fprintf(stderr, "DEBUG: use pre-generated key %s\n", keyPath);
			}
		}
	}
	return key;
}

static int fill_Dilithium_keyPair(DilithiumKey_t *dilithium, int algorithmVersion, char *keyPath) // added
{
	char *private_key;
	dilithium->publicKey.size = OQS_SIG_dilithium_2_length_public_key;
	dilithium->publicKey.buf = malloc(dilithium->publicKey.size);
	private_key = malloc(OQS_SIG_dilithium_2_length_secret_key);

	OQS_STATUS stat = OQS_SIG_dilithium_2_keypair(dilithium->publicKey.buf, private_key);
	if (stat != OQS_SUCCESS)
	{
		printf("\nError generating Dilithium key pair\n");
		return 0;
	}

	/* load Private Key */
	FILE *f = fopen(keyPath, "wb");
	if (f == NULL)
	{
		perror(keyPath);
		return -1;
	}

	fwrite(private_key, 1, OQS_SIG_dilithium_2_length_secret_key, f);

	fclose(f);

	/* load Public Key */
	void *key = NULL;
	keyPath = strcat(keyPath, EXT_PUB);

	f = fopen(keyPath, "wb");
	if (f == NULL)
	{
		perror(keyPath);
		return -1;
	}
	fwrite(dilithium->publicKey.buf, 1, OQS_SIG_dilithium_2_length_public_key, f);
	fclose(f);

	free(private_key);

	return 6;
}

static int fill_curve_point_eccP256(EccP256CurvePoint_t *point, ecc_curve_id curveType, char *keyPath)
{
	return fill_curve_point_eccP384((EccP384CurvePoint_t *)point, curveType, keyPath);
}

static void _fill_curve_point(EccP384CurvePoint_t *point, int fsize, const char *x, const char *y, int sign)
{
	OCTET_STRING_fromBuf(&point->choice.x_only, x, fsize);
	if (point->present == EccP384CurvePoint_PR_uncompressedP384)
	{
		OCTET_STRING_fromBuf(&point->choice.uncompressedP384.y, y, fsize);
	}
	else if (point->present == EccP384CurvePoint_PR_compressed_y_0 || point->present == EccP384CurvePoint_PR_compressed_y_1)
	{
		point->present = sign ? EccP384CurvePoint_PR_compressed_y_1 : EccP384CurvePoint_PR_compressed_y_0;
	}
}

static int fill_curve_point_eccP384(EccP384CurvePoint_t *point, ecc_curve_id curveType, char *keyPath)
{
	char x[48], y[48];
	int compressed_y;
	int fsize;
	int rc = -1;
	void *key = gen_or_load_public_key(curveType, keyPath);
	if (NULL == key)
		return -1;

	fsize = ecc_key_public(key, x, y, &compressed_y);

	if (fsize > 0)
	{
		_fill_curve_point(point, fsize, x, y, compressed_y);
		if (_debug)
		{
			char hex[256];
			*_bin2hex(hex, sizeof(hex), x, fsize) = 0;
			fprintf(stderr, "DEBUG: %s_pub.x=%s\n", keyPath, hex);
			*_bin2hex(hex, sizeof(hex), y, fsize) = 0;
			fprintf(stderr, "DEBUG: %s_pub.y=%s\n", keyPath, hex);
		}
		rc = 0;
	}
	else
	{
		fprintf(stderr, "ERROR: %s - unknown public key\n", keyPath);
	}
	ecc_key_free(key);
	return rc;
}

static void *fill_reconstruction_value(EccP256CurvePoint_t *point, void *r_key)
{
	char x[48], y[48];
	int compressed_y;

	void *eph = ecc_recvalue(r_key, x, y, &compressed_y);
	if (eph)
	{

		_fill_curve_point((EccP384CurvePoint_t *)point, sha256_hash_size, x, y, compressed_y);
	}
	return eph;
}

static void process_reconstruction_value(void *r_key, void *eph, void *ca, unsigned char *H, char *keyPath)
{
	// reconstruct private key
	void *u_key = ecc_reconstruct(r_key, eph, ca, H);
	if (u_key)
	{
		char *e = keyPath + strlen(keyPath);
		e = cstrcpy(e, EXT_VKEY);
		ecc_key_private_save(u_key, keyPath, _outKeyFormat);
		strcpy(e, EXT_PUB);
		ecc_key_public_save(u_key, keyPath, _outKeyFormat);
		ecc_key_free(u_key);
	}
}
