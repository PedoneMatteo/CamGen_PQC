#include <stdio.h>
#include "extensions.h"

int flag_PQC = 0;
char *pathMyCert = NULL;

int sha256_calculate(char *hash, const char *ptr, size_t len)
{
	SHA256_CTX ctx;
	SHA256_Init(&ctx);
	SHA256_Update(&ctx, ptr, len);
	SHA256_Final((unsigned char *)hash, &ctx);
	return 0;
}

void *search_private_Dilithium_key(char *pathCertificate)
{
	char *secretKey = malloc(OQS_SIG_dilithium_2_length_secret_key);

	char *path = malloc(strlen(pathCertificate) + 1);
	strcpy(path, pathCertificate);
	char *ext = cstrpathextension(path);
	strcpy(ext, ".vkey");

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

void *search_public_Dilithium_key(char *_keyPath)
{
	char *ext = cstrpathextension(_keyPath);
	strcpy(ext, ".vkey_pub");
	char *pubKey = malloc(OQS_SIG_dilithium_2_length_public_key);
	// char *path = cvstrdup(_keyPath, "/", sName, ".vkey", NULL);

	FILE *f = fopen(_keyPath, "rb");

	if (f == NULL)
	{
		fprintf(stderr, "Error: impossible to open the file %s\n", _keyPath);
		free(pubKey);
		// free(_keyPath);
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
	// free(_keyPath);

	return pubKey;
}

// Funzione per calcolare il tempo corrente
double get_time()
{
	struct timespec ts;
	clock_gettime(CLOCK_MONOTONIC, &ts);
	return ts.tv_sec + ts.tv_nsec / 1e9;
}

int cont = 1;
FILE *file = NULL; // Dichiarazione del file globale
double sumDil=0.0,sumEcdsa=0.0,sumFal=0.0,sumSh=0.0;
double avgDil, avgEcdsa, avgFal, avgSh;

void compare_sigTime(char *hash, int hashLen, double dilithiumTime)
{

	double startTime, endTime;
	printf("\n				 cont = %d\n\n", cont);
	// Apertura del file CSV al primo ciclo
	if (cont == 1)
	{
		file = fopen("signature_times.csv", "w");
		if (!file)
		{
			fprintf(stderr, "Error opening file\n");
			return;
		}
		// Scrittura dell'intestazione delle colonne
		fprintf(file, "Dilithium Time,ECDSA Time,Falcon Time,Sphincs Time\n");
	}

	// ECDSA signature
	FSCrypt *cript = FSCrypt_FindEngine("openssl");
	FSPrivateKey *privKey = FSKey_Generate(cript, FS_NISTP256, NULL);
	FSSignature *sigEcdsa = malloc(sizeof(FSSignature));
	sigEcdsa->curve = FS_NISTP256;
	sigEcdsa->point.x = malloc(32 * sizeof(uint8_t));
	sigEcdsa->point.y = malloc(32 * sizeof(uint8_t));
	sigEcdsa->s = malloc(64 * sizeof(uint8_t));

	startTime = get_time();
	bool res = FSSignature_Sign(cript, sigEcdsa, privKey, hash);
	endTime = get_time();
	double ecdsaTime = endTime - startTime;

	if (res < 0)
	{
		fprintf(stderr, "Error generating ECDSA signature\n");
		free(sigEcdsa->point.x);
		free(sigEcdsa->point.y);
		free(sigEcdsa->s);
		free(sigEcdsa);
		return;
	}

	printf("Ecdsa sign time:      %f seconds\n", ecdsaTime);

	free(sigEcdsa->point.x);
	free(sigEcdsa->point.y);
	free(sigEcdsa->s);
	free(sigEcdsa);

	// Falcon signature
	uint8_t *publicKey = malloc(OQS_SIG_falcon_512_length_public_key);
	uint8_t *secretKey = malloc(OQS_SIG_falcon_512_length_secret_key);
	uint8_t *signature = malloc(OQS_SIG_falcon_512_length_signature);
	size_t signatureLen = OQS_SIG_falcon_512_length_signature;

	OQS_STATUS check = OQS_SIG_falcon_512_keypair(publicKey, secretKey);
	if (check != OQS_SUCCESS)
	{
		fprintf(stderr, "Error generating Falcon keys\n");
		return;
	}

	startTime = get_time();
	check = OQS_SIG_falcon_512_sign(signature, &signatureLen, hash, hashLen, secretKey);
	endTime = get_time();
	double falconTime = endTime - startTime;

	if (check != OQS_SUCCESS)
	{
		fprintf(stderr, "Error generating Falcon signatures\n");
		return;
	}

	printf("Falcon sign time:     %f seconds\n", falconTime);

	free(publicKey);
	free(secretKey);
	free(signature);

	// Sphincs signature
	publicKey = malloc(OQS_SIG_sphincs_shake_128s_simple_length_public_key);
	secretKey = malloc(OQS_SIG_sphincs_shake_128s_simple_length_secret_key);
	signature = malloc(OQS_SIG_sphincs_shake_128s_simple_length_signature);
	signatureLen = OQS_SIG_sphincs_shake_128s_simple_length_signature;

	check = OQS_SIG_sphincs_shake_128s_simple_keypair(publicKey, secretKey);
	if (check != OQS_SUCCESS)
	{
		fprintf(stderr, "Error generating Sphincs keys\n");
		return;
	}

	startTime = get_time();
	check = OQS_SIG_sphincs_shake_128s_simple_sign(signature, &signatureLen, hash, hashLen, secretKey);
	endTime = get_time();
	double sphincsTime = endTime - startTime;

	if (check != OQS_SUCCESS)
	{
		fprintf(stderr, "Error generating Sphincs signatures\n");
		return;
	}

	printf("Sphincs sign time:    %f seconds\n", sphincsTime);

	free(publicKey);
	free(secretKey);
	free(signature);

	sumDil+=dilithiumTime; 
	sumEcdsa+=ecdsaTime; 
	sumFal+=falconTime; 
	sumSh+=sphincsTime;
	if (cont <= 2000)
	{
		// Scrittura dei tempi nel file CSV
		fprintf(file, "%f;%f;%f;%f\n", dilithiumTime, ecdsaTime, falconTime, sphincsTime);
		sumDil+=dilithiumTime; 
		sumEcdsa+=ecdsaTime; 
		sumFal+=falconTime; 
		sumSh+=sphincsTime;
	}
	// Se il contatore arriva a 200, chiudi il file
	if (cont == 2000)
	{
		fclose(file);
		file = NULL; // Resetta il puntatore per evitare problemi
		printf("File signature_times.csv closed after 200 entries.\n");
		avgDil = (double)sumDil/(cont);
		avgEcdsa = (double) sumEcdsa/(cont);
		avgFal = (double) sumFal/(cont);
		avgSh = (double) sumSh/(cont);
		printf("the averages are:\n	- Ecdsa: %f\n - Dilithium: %f\n - Falcon: %f\n- Shpincs: %f\n\n", avgEcdsa, avgDil, avgFal, avgSh);
	}
	cont++;
}