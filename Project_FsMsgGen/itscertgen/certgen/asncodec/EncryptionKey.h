/*
 * Generated by asn1c-0.9.29-DF (http://lionet.info/asn1c)
 * From ASN.1 module "Ieee1609Dot2BaseTypes"
 * 	found in "asn1/ieee1609.2/Ieee1609Dot2BaseTypes.asn"
 * 	`asn1c -fcompound-names -no-gen-BER -no-gen-JER -no-gen-UPER -no-gen-APER -no-gen-random-fill -no-gen-example -pdu=EtsiTs103097Data -pdu=EnrolmentRequestMessage -pdu=EnrolmentResponseMessage -pdu=AuthorizationRequestMessage -pdu=AuthorizationRequestMessageWithPop -pdu=AuthorizationResponseMessage -pdu=CertificateRevocationListMessage -pdu=TlmCertificateTrustListMessage -pdu=RcaCertificateTrustListMessage -pdu=AuthorizationValidationRequestMessage -pdu=AuthorizationValidationResponseMessage -pdu=CaCertificateRequestMessage -pdu=EtsiTs103097Certificate`
 */


/* Including external dependencies */
#include "PublicEncryptionKey.h"
#include "SymmetricEncryptionKey.h"
#include "DilithiumKey.h"
#include <constr_CHOICE.h>
#ifndef	_EncryptionKey_H_
#define	_EncryptionKey_H_


#include <asn_application.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum EncryptionKey_PR {
	EncryptionKey_PR_NOTHING,	/* No components present */
	EncryptionKey_PR_public,
	EncryptionKey_PR_symmetric,
	EncryptionKey_PR_dilithiumKey
} EncryptionKey_PR;

/* EncryptionKey */
typedef struct EncryptionKey {
	EncryptionKey_PR present;
	union EncryptionKey_u {
		PublicEncryptionKey_t	 Public;
		SymmetricEncryptionKey_t	 symmetric;
		DilithiumKey_t	 dilithiumKey;
	} choice;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} EncryptionKey_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_EncryptionKey;
extern asn_CHOICE_specifics_t asn_SPC_EncryptionKey_specs_1;
extern asn_TYPE_member_t asn_MBR_EncryptionKey_1[3];

#ifdef __cplusplus
}
#endif

#endif	/* _EncryptionKey_H_ */
#include <asn_internal.h>
