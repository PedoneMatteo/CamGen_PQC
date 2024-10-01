#include <stdio.h>
#include "extensions.h"

int flag_PQC = 0;
char *pathMyCert=NULL;

int sha256_calculate(char *hash, const char *ptr, size_t len)
{
	SHA256_CTX ctx;
	SHA256_Init(&ctx);
	SHA256_Update(&ctx, ptr, len);
	SHA256_Final((unsigned char *)hash, &ctx);
	return 0;
}


void *search_private_Dilithium_key(char* pathCertificate)
{
	char *secretKey = malloc(OQS_SIG_dilithium_2_length_secret_key);

	char* path = malloc(strlen(pathCertificate)+1);
	strcpy(path, pathCertificate);
	char* ext = cstrpathextension(path);
	strcpy(ext,".vkey");

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

// Funzione per calcolare il tempo corrente
double get_time() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + ts.tv_nsec / 1e9;
}