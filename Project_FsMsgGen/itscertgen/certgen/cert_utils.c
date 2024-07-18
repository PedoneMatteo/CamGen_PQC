#include "asn_application.h"
#include "EtsiTs103097Certificate.h"

HashAlgorithm_t CertificateWholeHashAlgorithm(const EtsiTs103097Certificate_t * const cert, const char * name)
{
    HashAlgorithm_t alg;
    
    if(cert->issuer.present == IssuerIdentifier_PR_self){
        alg = cert->issuer.choice.self;
    } else if(cert->type == CertificateType_implicit){
        switch(cert->issuer.present){
            case IssuerIdentifier_PR_sha384AndDigest:
                alg = HashAlgorithm_sha384;
                break;
            case IssuerIdentifier_PR_sha256AndDigest:
                alg = HashAlgorithm_sha256;
                break;
            default:
                if(name) fprintf(stderr, "%s: ERROR: Unknow issuer hash algorithm\n", name);
                return -1;
        }
    } else if(cert->type == CertificateType_explicit){
        switch(cert->toBeSigned.verifyKeyIndicator.choice.verificationKey.present){
            case PublicVerificationKey_PR_ecdsaNistP256:
            case PublicVerificationKey_PR_ecdsaBrainpoolP256r1:
                alg = HashAlgorithm_sha256;
                break;
            case PublicVerificationKey_PR_ecdsaBrainpoolP384r1:
                alg = HashAlgorithm_sha384;
                break;
            default:
                if(name) fprintf(stderr, "%s: ERROR: Unknow verification key algorithm\n", name);
                return -1;
        }
    } else {
        if(name) fprintf(stderr, "%s: ERROR: Unknow certificate type\n", name);
        return -1;
    }
    return alg;
}