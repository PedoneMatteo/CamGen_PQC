#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <stdio.h>
#include "../cshared/copts.h"
#include "../cshared/cstr.h"

#include "EtsiTs103097Certificate.h"

#if defined(CYGWIN) || !defined(WIN32)
#include <glob.h>

#if !defined(_GLOB_H_) && 0
#define _GLOB_H_
typedef struct {
	size_t gl_pathc;	/* Count of total paths so far. */
	size_t gl_matchc;	/* Count of paths matching pattern. */
	size_t gl_offs;		/* Reserved at beginning of gl_pathv. */
	int gl_flags;		/* Copy of flags parameter to glob. */
	char **gl_pathv;	/* List of paths matching pattern. */
}glob_t;

int	 glob (const char * pattern,  int flags , int (*)(const char *, int), glob_t * gl);
void globfree (glob_t *);
int	 glob_pattern_p (const char *, int);

#endif // _GLOB_H_
#endif

#include "ecc_api.h"

#define CERT_MAX_SIZE 0x10000

static const char * _outPath = "-";
static int _canonicalize = 1;
static int _fnames = 0;
static FILE * _out;

static copt_t _options [] = {
	{ "h?", "help", COPT_HELP, NULL,                 "Print this help page" },
	{ "o",  "out", COPT_STR, (void*)&_outPath,         "Output path [stdout by default]" },
	{ "C",  "no-canonicalize", COPT_BOOLI, (void*)&_canonicalize,         "No canonicalize" },
	{ "n",  "fnames", COPT_BOOL, (void*)&_fnames,         "Use file names instead of IDs" },
	{ NULL, NULL, COPT_END, NULL, NULL }
};

char * _bin2hex(char * hex, size_t hlen, const char * bin, size_t blen);

HashAlgorithm_t CertificateWholeHashAlgorithm(const EtsiTs103097Certificate_t * const cert, const char * name);

int process_cert(const pchar_t * name, uint8_t * buf, const uint8_t * end);

static ecc_curve_id _pk_type_to_curveid[] = {
	0,
	ecies_nistp256,       //Signature_PR_ecdsaNistP256Signature,
	ecies_brainpoolp256r, //Signature_PR_ecdsaBrainpoolP256r1Signature,
	ecies_brainpoolp384r  //Signature_PR_ecdsaBrainpoolP384r1Signature
};

int main(int argc, char ** argv)
{
	//parse options
	argc = coptions(argc, argv, COPT_HELP_NOVALUES , _options);
	if(argc < 2){
		if(argc < 0 && (0-argc)<((sizeof(_options)/sizeof(_options[0]))-1)){
			printf("Unknown option %s\n", argv[0-argc]);
		}
		const char * a = strrchr(argv[0], '/');
		if (a == NULL) a = argv[0];
		coptions_help(stdout, a, COPT_HELP_NOVALUES, _options, "... certificate files ...");
		return -1;
	}

	if(ecc_api_init()){
		fprintf(stderr, "Cannot initialize ECC engine\n");
		return -1;
	}

    if(!cstrequal("-", _outPath)){
        _out = fopen(_outPath, "wb");
        if(_out == NULL){
            perror(_outPath);
            return -1;
        }
    }else{
        _out = stdout;
    }

	char * buf = malloc(CERT_MAX_SIZE);
	char * ebuf;

    for(int argi=1; argi<argc; argi++){
    	ebuf = cstrnload(buf, CERT_MAX_SIZE, argv[argi]);
        if(ebuf && (ebuf - buf) > 0){
            const pchar_t* fname = cstrlastpathelement(argv[argi]);
            if(!_fnames) *cstrpathextension(fname) = 0;
            process_cert(fname, buf, ebuf);
        }else{
            // check if it is a mask
#if defined(_GLOB_H_)
            glob_t gl = {0};
            if(0 == glob( argv[argi], 0, NULL, &gl )){
                for (size_t i=0; i<gl.gl_pathc; i++){
                    ebuf = cstrnload(buf, CERT_MAX_SIZE, gl.gl_pathv[i]);
                    if (ebuf && (ebuf - buf) > 0) {
                        const pchar_t* fname = cstrlastpathelement(gl.gl_pathv[i]);
                        if(!_fnames) *cstrpathextension(fname) = 0;
                        process_cert(fname, buf, ebuf);
                    }
                }
                globfree (&gl);
            }
#elif defined(WIN32)
// try use the FindFile approach
            WIN32_FIND_DATA fd;
            size_t len = cstrlen(argv[argi]);
            pchar_t * path = cstralloc(len + MAX_PATH);
            cstrcpy(path, argv[argi]);
            pchar_t * fname = cstrlastpathelement(path);
            HANDLE h = FindFirstFile(path, &fd);
    		if(INVALID_HANDLE_VALUE != h) do {
                cstrcpy(fname, fd.cFileName);
                ebuf = cstrnload(buf, CERT_MAX_SIZE, path);
                if(ebuf && (ebuf - buf) > 0){
                    if(!_fnames) *cstrpathextension(fname) = 0;
                    process_cert(fname, buf, ebuf);            
                }
            }while(FindNextFile(h, &fd));
            FindClose(h);
#endif
        }
    }
    free(buf);
    return 0;
}

typedef struct its_asn_codec_ctx_s {
    asn_codec_ctx_t ctx;
    ecc_curve_id curve;
    size_t mcount;
    struct {
        uint8_t* ptr;
        size_t   shift;
    }move[8];
}its_asn_codec_ctx_t;

static int update_ecc_curve(ecc_curve_id curve, EccP256CurvePoint_t* p)
{
    if (p->present == EccP256CurvePoint_PR_uncompressedP256 || p->present == EccP256CurvePoint_PR_x_only) {
        void* key = ecc_key_public_set(curve, p->present - 1, p->choice.uncompressedP256.x.buf, p->choice.uncompressedP256.y.buf);
        if (key) {
            int sign;
            char x[48], y[48];
            ecc_key_public(key, &x[0], &y[0], &sign);
            p->present = EccP256CurvePoint_PR_compressed_y_0 + (sign ? 1 : 0);
            ecc_key_free(key);
            return 1;
        }
    }
    return 0;
}

int process_cert(const pchar_t * name, uint8_t * buf, const uint8_t * ebuf)
{
    // determine the hash alg
    EtsiTs103097Certificate_t * cert = NULL;
	asn_dec_rval_t rc_d;
    its_asn_codec_ctx_t ctx = {{0}};
    int updated = 0;
    HashAlgorithm_t halg;
    rc_d = asn_decode(&ctx.ctx, ATS_BASIC_OER, &asn_DEF_EtsiTs103097Certificate, (void**)&cert, buf, ebuf - buf);
	if (rc_d.code != RC_OK){
		fprintf(stderr, "%s: ERROR: failed to load at position %d\n  %.30s\n", name, (int)rc_d.consumed, buf + rc_d.consumed);
		return -1;
	}

    if (_canonicalize) {
        if (cert->toBeSigned.verifyKeyIndicator.present == VerificationKeyIndicator_PR_verificationKey) {
            // use ecdsaBrainpoolP256r1. other variantes has the same structure
            updated += update_ecc_curve(_pk_type_to_curveid[cert->toBeSigned.verifyKeyIndicator.choice.verificationKey.present],
                &cert->toBeSigned.verifyKeyIndicator.choice.verificationKey.choice.ecdsaNistP256);
        }
        else if (cert->toBeSigned.verifyKeyIndicator.present == VerificationKeyIndicator_PR_reconstructionValue) {
            if (cert->toBeSigned.verifyKeyIndicator.choice.reconstructionValue.present != EccP256CurvePoint_PR_compressed_y_0 &&
                cert->toBeSigned.verifyKeyIndicator.choice.reconstructionValue.present != EccP256CurvePoint_PR_compressed_y_1) {
                fprintf(stderr, "%s: ERROR: reconstruction value should be in compressed form\n", name);
                return -1;
            }
        }
        if (cert->toBeSigned.encryptionKey) {
            updated += update_ecc_curve(_pk_type_to_curveid[cert->toBeSigned.encryptionKey->publicKey.present],
                &cert->toBeSigned.encryptionKey->publicKey.choice.eciesNistP256);
        }

        if (cert->signature) {
            if (cert->signature->choice.ecdsaNistP256Signature.rSig.present != EccP256CurvePoint_PR_x_only) {
                cert->signature->choice.ecdsaNistP256Signature.rSig.present = EccP256CurvePoint_PR_x_only;
                updated++;
            }
        }
        if (updated) {
            asn_enc_rval_t rc_e = asn_encode_to_buffer(NULL, ATS_CANONICAL_OER, &asn_DEF_EtsiTs103097Certificate, cert, buf, CERT_MAX_SIZE);
            if (rc_e.encoded < 0) {
                fprintf(stderr, "%s: ERROR: OER encoding failed for canonicalized certificate at type %s\n", name, rc_e.failed_type->name);
                return -1;
            }
            rc_d.consumed = rc_e.encoded;
        }
    }

    halg = CertificateWholeHashAlgorithm(cert, name);

    ASN_STRUCT_FREE(asn_DEF_EtsiTs103097Certificate, cert);

    uint8_t hash[256];
    uint8_t * digest;
    switch(halg) {
        case HashAlgorithm_sha256:
            sha256_calculate(&hash[0], buf, rc_d.consumed);
            digest = &hash[32 - 8];
            break;
        case HashAlgorithm_sha384:
            sha384_calculate(&hash[0], buf, rc_d.consumed);
            digest = &hash[48 - 8];
            break;
        default:
            return -1;
    }
    char hex[20];
    * cstr_bin2hex(hex, sizeof(hex), digest, 8) = 0;
    fprintf(_out, "%s %s\n", hex, name);
    return 0;
}
