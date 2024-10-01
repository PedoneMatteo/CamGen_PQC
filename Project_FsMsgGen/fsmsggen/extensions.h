#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/ec.h>
#include <openssl/pem.h>
#include <openssl/sha.h>
#include <openssl/ecdsa.h> 
#include <time.h>

#include "../liboqs/build/include/oqs/sig_dilithium.h"
#include "./cshared/cstr.h"
#include "./fitsec2-rel/fitsec.h"

#define CERT_MAX_SIZE 0x10000
#define sha256_hash_size 32

extern int flag_PQC;  

int sha256_calculate(char *hash, const char *ptr, size_t len);
void *search_private_Dilithium_key(char* pathCertificate);
void *search_public_Dilithium_key(char* _keyPath);
double get_time();
//int OCTET_STRING_fromBuf(OCTET_STRING_t *st, const char *str, int len);

typedef struct DilithiumKey {
	uint8_t*	 publicKey;
} DilithiumKey_t;

typedef struct DilithiumSignature {
	uint8_t*	 signature;
} DilithiumSignature_t;

typedef struct
{
	uint16_t psid;
	struct
	{
        uint8_t bitmapSspLength;
		uint8_t *bitmapSsp;
	} ssp;
} PsidSsp;

typedef struct
{
    uint32_t initialized;
	uint8_t *sspValue;
	uint8_t *sspBitmask;
	size_t sspValueLength;
	size_t sspBitmaskLength;
} BitmapSspRange;

typedef struct
{
	uint16_t psid;
	BitmapSspRange sspRange;
} PsidSspRange;

typedef struct
{
	PsidSspRange *psidSspRanges;
	size_t numRanges;
} ExplicitPermissions;

typedef struct
{
	ExplicitPermissions explicitPermissions;
} SubjectPermissions;

typedef struct
{
	SubjectPermissions subjectPermissions;
	uint8_t minChainLength;
	uint8_t chainLengthRange;
	uint8_t eeType;
} PsidGroupPermissions;

typedef struct
{
	PsidGroupPermissions *psidGroupPermissions;
	size_t numGroupPermissions;
} CertIssuePermissions;

typedef struct
{
	FSSymmAlg supportedSymmAlg; 
	FSPublicKey publicKey;
} EncryptionKey;

typedef struct
{
    union{
        FSPublicKey FSverificationKey;
        DilithiumKey_t DILverificationKey;
    }val;
} VerifyKeyIndicator;

typedef struct
{
	struct {
        uint8_t idType;
        union{
            struct{
                uint8_t len;
                uint8_t *val;
            }name
        }id;
    }id_Value;
	uint8_t cracaId[7]; // "00 00 00"
	uint8_t crlSeries[2];
	struct
	{
		uint8_t start[4];
        uint8_t durationType;
		struct
		{
			uint8_t hours[2];
		} duration;
	} validityPeriod;
	uint8_t assuranceLevel;
	PsidSsp *appPermissions;
	size_t numAppPermissions;
	union{
        CertIssuePermissions certIssuePermissions;
    };
	EncryptionKey encryptionKey;
	VerifyKeyIndicator verifyKeyIndicator;
} ToBeSigned;

typedef struct
{
	uint8_t issuerType;
    union{
        uint8_t *Digest;
    }sha256
} Issuer;

typedef struct
{
	uint8_t version;
	uint8_t type;
	Issuer issuer;
	ToBeSigned toBeSigned;
    union{
        FSSignature FSsignature;
        DilithiumSignature_t DILsignature;
    }sig;
} EtsiExtendedCertificate;

extern EtsiExtendedCertificate *extendedCert;


typedef struct extCert{
	unsigned char buf[5000];
	size_t size;
}extCert;
extern extCert myCert; 

extern char *pathMyCert;