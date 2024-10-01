#include <stdio.h>
#include <time.h>
#include <openssl/evp.h>
#include <openssl/ec.h>
#include <openssl/ecdsa.h>
#include <openssl/sha.h>
#include <openssl/core_names.h>
#include <string.h>
#include "../liboqs/build/include/oqs/sig_dilithium.h"
#include "../liboqs/build/include/oqs/sig_falcon.h"
#include "../liboqs/build/include/oqs/sig_sphincs.h"


double start_time;
double end_time;

// Funzione per calcolare il tempo corrente
double get_time() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + ts.tv_nsec / 1e9;
}

// Funzione per firmare con ECC utilizzando le nuove API di OpenSSL 3.0
void sign_with_ecc(const unsigned char *message, size_t message_len) {
    EVP_PKEY *pkey = NULL;
    EVP_PKEY_CTX *pctx = NULL;
    EVP_MD_CTX *mdctx = NULL;
    unsigned char *sig;
    size_t siglen = 0;

    // Creazione del contesto per la chiave
    pctx = EVP_PKEY_CTX_new_id(EVP_PKEY_EC, NULL);
    if (pctx == NULL || EVP_PKEY_keygen_init(pctx) <= 0) {
        printf("Error initializing keygen context\n");
        goto cleanup;
    }

    // Seleziona la curva ECC (NID_X9_62_prime256v1)
    if (EVP_PKEY_CTX_set_ec_paramgen_curve_nid(pctx, NID_X9_62_prime256v1) <= 0) {
        printf("Error setting curve\n");
        goto cleanup;
    }

    // Genera la chiave privata ECC
    if (EVP_PKEY_keygen(pctx, &pkey) <= 0) {
        printf("Error generating key\n");
        goto cleanup;
    }

    // Creazione del contesto per la firma
    mdctx = EVP_MD_CTX_new();
    if (mdctx == NULL || EVP_DigestSignInit(mdctx, NULL, EVP_sha256(), NULL, pkey) <= 0) {
        printf("Error initializing digest sign\n");
        goto cleanup;
    }

    // Calcola la firma
    if (EVP_DigestSign(mdctx, NULL, &siglen, message, message_len) <= 0) {
        printf("Error computing signature size\n");
        goto cleanup;
    }

    sig = (unsigned char *)OPENSSL_malloc(siglen);
    if (sig == NULL) {
        printf("Error allocating memory for signature\n");
        goto cleanup;
    }
    start_time = get_time();
    int check = EVP_DigestSign(mdctx, sig, &siglen, message, message_len);
    end_time = get_time();

    if ( check <= 0) {
        printf("Error computing signature\n");
    } else {
        printf("ECC signature succeeded (size: %zu bytes)\n", siglen);
    }
    
    printf("ECC sign time: %f seconds\n", end_time - start_time);
   
   // Verify the signature
    EVP_MD_CTX *verify_ctx = EVP_MD_CTX_new();
    if (verify_ctx == NULL || EVP_DigestVerifyInit(verify_ctx, NULL, EVP_sha256(), NULL, pkey) <= 0) {
        printf("Error initializing signature verification\n");
        goto cleanup;
    }

    start_time = get_time();
    if (EVP_DigestVerify(verify_ctx, sig, siglen, message, message_len) == 1) {
        printf("Signature verification succeeded\n");
    } else {
        printf("Signature verification failed\n");
    }
    end_time = get_time();
    printf("ECC verification time: %f seconds\n\n", end_time - start_time);

cleanup:
    EVP_MD_CTX_free(mdctx);
    EVP_PKEY_free(pkey);
    EVP_PKEY_CTX_free(pctx);
    OPENSSL_free(sig);
}

// Funzione simulata per firmare con Dilithium 
void sign_with_dilithium(const unsigned char *message, size_t message_len) {
    uint8_t *publicKey = malloc(OQS_SIG_dilithium_2_length_public_key);
    uint8_t *secretKey = malloc(OQS_SIG_dilithium_2_length_secret_key);
    uint8_t *signature = malloc(OQS_SIG_dilithium_2_length_signature);
    size_t signatureLen = OQS_SIG_dilithium_2_length_signature;
    OQS_STATUS check = OQS_SIG_dilithium_2_keypair(publicKey, secretKey);
    if(check!=OQS_SUCCESS){
        fprintf(stderr, "Error generating Dilithium keys\n");
        return;
    }
    start_time = get_time();
    check = OQS_SIG_dilithium_2_sign(signature, &signatureLen, message, message_len, secretKey);
    end_time = get_time();
    if(check!=OQS_SUCCESS){
        fprintf(stderr, "Error generating Dilithium signatures\n");
        return;
    }else{
        printf("Dilithium signature succeeded (size: %zu bytes)\n", signatureLen);
    }
    
    printf("Dilithium sign time: %f seconds\n", end_time - start_time);

    start_time = get_time();
    check = OQS_SIG_dilithium_2_verify(message, message_len, signature, signatureLen, publicKey);
    end_time = get_time();
    if(check!=OQS_SUCCESS){
        fprintf(stderr, "Error verifying Dilithium signatures\n");
        return;
    }else{
        printf("Dilithium verification succeeded (size: %zu bytes)\n", signatureLen);
    }
    printf("Dilithium verification time: %f seconds\n\n", end_time - start_time);
}

// Funzione simulata per firmare con Falcon
void sign_with_falcon(const unsigned char *message, size_t message_len) {
    uint8_t *publicKey = malloc(OQS_SIG_falcon_512_length_public_key);
    uint8_t *secretKey = malloc(OQS_SIG_falcon_512_length_secret_key);
    uint8_t *signature = malloc(OQS_SIG_falcon_512_length_signature);
    size_t signatureLen = OQS_SIG_falcon_512_length_signature;

    OQS_STATUS check = OQS_SIG_falcon_512_keypair(publicKey, secretKey);
    if(check!=OQS_SUCCESS){
        fprintf(stderr, "Error generating Falcon keys\n");
        return;
    }

    start_time = get_time();
    check = OQS_SIG_falcon_512_sign(signature, &signatureLen, message, message_len, secretKey);
    end_time = get_time();
    if(check!=OQS_SUCCESS){
        fprintf(stderr, "Error generating Falcon signatures\n");
        return;
    }else{
        printf("Falcon signature succeeded (size: %zu bytes)\n", signatureLen);
    }
    printf("Falcon sign time: %f seconds\n", end_time - start_time);

    start_time = get_time();
    check = OQS_SIG_falcon_512_verify(message, message_len, signature, signatureLen, publicKey);
    end_time = get_time();
    if(check!=OQS_SUCCESS){
        fprintf(stderr, "Error verifying Falcon signatures\n");
        return;
    }else{
        printf("Falcon verification succeeded (size: %zu bytes)\n", signatureLen);
    }
    printf("Falcon verification time: %f seconds\n\n", end_time - start_time);
}

// Funzione simulata per firmare con Sphincs
void sign_with_sphincs(const unsigned char *message, size_t message_len) {
    uint8_t *publicKey = malloc(OQS_SIG_sphincs_shake_128s_simple_length_public_key);
    uint8_t *secretKey = malloc(OQS_SIG_sphincs_shake_128s_simple_length_secret_key);
    uint8_t *signature = malloc(OQS_SIG_sphincs_shake_128s_simple_length_signature);
    size_t signatureLen = OQS_SIG_sphincs_shake_128s_simple_length_signature;

    OQS_STATUS check = OQS_SIG_sphincs_shake_128s_simple_keypair(publicKey, secretKey);
    if(check!=OQS_SUCCESS){
        fprintf(stderr, "Error generating Sphincs keys\n");
        return;
    }

    start_time = get_time();
    check = OQS_SIG_sphincs_shake_128s_simple_sign(signature, &signatureLen, message, message_len, secretKey);
    end_time = get_time();
    if(check!=OQS_SUCCESS){
        fprintf(stderr, "Error generating Sphincs signatures\n");
        return;
    }else{
        printf("Sphincs signature succeeded (size: %zu bytes)\n", signatureLen);
    }
    printf("Sphincs sign time: %f seconds\n", end_time - start_time);

    start_time = get_time();
    check = OQS_SIG_sphincs_shake_128s_simple_verify(message, message_len, signature, signatureLen, publicKey);
    end_time = get_time();
    if(check!=OQS_SUCCESS){
        fprintf(stderr, "Error verifying Sphincs signatures\n");
        return;
    }else{
        printf("Sphincs verification succeeded (size: %zu bytes)\n", signatureLen);
    }
    printf("Sphincs verification time: %f seconds\n\n", end_time - start_time);
}

int main() {
    const unsigned char message[] = {0x03,0x67,0x89,0x21,0x23,0x01,0xA5,0x2E};
    size_t message_len = 8;

    // Misura il tempo per ECC
    sign_with_ecc(message, message_len);

    // Misura il tempo per Dilithium (simulato)
    sign_with_dilithium(message, message_len);
  
    sign_with_falcon(message, message_len);

    sign_with_sphincs(message, message_len);
    
    return 0;
}
