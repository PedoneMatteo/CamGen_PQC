#include <stdio.h>
#include "extensions.h"


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
    
    printf("\n path = %s\n", path);
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
