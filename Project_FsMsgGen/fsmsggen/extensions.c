#include <stdio.h>
#include "extensions.h"

int flag_PQC = 0;
//
//typedef struct OCTET_STRING {
//	uint8_t *buf;	/* Buffer with consecutive OCTET_STRING bits */
//	size_t size;	/* Size of the buffer */
//
//	asn_struct_ctx_t _asn_ctx;	/* Parsing across buffer boundaries */
//} OCTET_STRING_t;
//
//typedef OCTET_STRING_t	 HashedId8_t;
//
//
//int OCTET_STRING_fromBuf(OCTET_STRING_t *st, const char *str, int len) {
//	void *buf;
//
//	if(st == 0 || (str == 0 && len)) {
//		errno = EINVAL;
//		return -1;
//	}
//
//	/*
//	 * Clear the OCTET STRING.
//	 */
//	if(str == NULL) {
//		FREEMEM(st->buf);
//		st->buf = 0;
//		st->size = 0;
//		return 0;
//	}
//
//	/* Determine the original string size, if not explicitly given */
//	if(len < 0)
//		len = strlen(str);
//
//	/* Allocate and fill the memory */
//	buf = MALLOC(len + 1);
//	if(buf == NULL)
//		return -1;
//
//	memcpy(buf, str, len);
//	((uint8_t *)buf)[len] = '\0';	/* Couldn't use memcpy(len+1)! */
//	FREEMEM(st->buf);
//	st->buf = (uint8_t *)buf;
//	st->size = len;
//
//	return 0;
//}
//
int sha256_calculate(char *hash, const char *ptr, size_t len)
{
	SHA256_CTX ctx;
	SHA256_Init(&ctx);
	SHA256_Update(&ctx, ptr, len);
	SHA256_Final((unsigned char *)hash, &ctx);
	return 0;
}


void *search_private_Dilithium_key(char* _keyPath, const char *sName)
{
	char *secretKey = malloc(OQS_SIG_dilithium_2_length_secret_key);
	char *path = cvstrdup(_keyPath, "/", sName, ".vkey", NULL);
	FILE *f = fopen(path, "rb");
  
	if (f == NULL)
	{
		fprintf(stderr, "Error: impossible to open the file %s\n", path);
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

void *search_public_Dilithium_key(char* _keyPath)
{
	char* ext = cstrpathextension(_keyPath);
	strcpy(ext,".vkey_pub");
	char *pubKey = malloc(OQS_SIG_dilithium_2_length_public_key);
	//char *path = cvstrdup(_keyPath, "/", sName, ".vkey", NULL);
	printf("\n\n keyPath pubKey = %s\n\n", _keyPath);
	FILE *f = fopen(_keyPath, "rb");
  
	if (f == NULL)
	{
		fprintf(stderr, "Error: impossible to open the file %s\n", _keyPath);
		free(pubKey);
		//free(_keyPath);
		return NULL;
	}

	// read content of file
	size_t bytesRead = fread(pubKey, 1, OQS_SIG_dilithium_2_length_public_key, f);

	if (bytesRead != OQS_SIG_dilithium_2_length_public_key)
	{
		fprintf(stderr, "Error: wrong len of bytes read = %ld - %s\n", bytesRead, _keyPath);
		free(pubKey);
		pubKey = NULL;
	}

	fclose(f);
	//free(_keyPath);

	return pubKey;
}