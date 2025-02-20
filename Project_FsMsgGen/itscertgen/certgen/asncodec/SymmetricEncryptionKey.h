/*
 * Generated by asn1c-0.9.29-DF (http://lionet.info/asn1c)
 * From ASN.1 module "Ieee1609Dot2BaseTypes"
 * 	found in "asn1/ieee1609.2/Ieee1609Dot2BaseTypes.asn"
 * 	`asn1c -fcompound-names -no-gen-BER -no-gen-JER -no-gen-UPER -no-gen-APER -no-gen-random-fill -no-gen-example -pdu=EtsiTs103097Data -pdu=EnrolmentRequestMessage -pdu=EnrolmentResponseMessage -pdu=AuthorizationRequestMessage -pdu=AuthorizationRequestMessageWithPop -pdu=AuthorizationResponseMessage -pdu=CertificateRevocationListMessage -pdu=TlmCertificateTrustListMessage -pdu=RcaCertificateTrustListMessage -pdu=AuthorizationValidationRequestMessage -pdu=AuthorizationValidationResponseMessage -pdu=CaCertificateRequestMessage -pdu=EtsiTs103097Certificate`
 */


/* Including external dependencies */
#include <OCTET_STRING.h>
#include <constr_CHOICE.h>
#ifndef	_SymmetricEncryptionKey_H_
#define	_SymmetricEncryptionKey_H_


#include <asn_application.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum SymmetricEncryptionKey_PR {
	SymmetricEncryptionKey_PR_NOTHING,	/* No components present */
	SymmetricEncryptionKey_PR_aes128Ccm,
	/* Extensions may appear below */
	SymmetricEncryptionKey_PR_sm4Ccm
} SymmetricEncryptionKey_PR;

/* SymmetricEncryptionKey */
typedef struct SymmetricEncryptionKey {
	SymmetricEncryptionKey_PR present;
	union SymmetricEncryptionKey_u {
		OCTET_STRING_t	 aes128Ccm;
		/*
		 * This type is extensible,
		 * possible extensions are below.
		 */
		OCTET_STRING_t	 sm4Ccm;
	} choice;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} SymmetricEncryptionKey_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_SymmetricEncryptionKey;
extern asn_CHOICE_specifics_t asn_SPC_SymmetricEncryptionKey_specs_1;
extern asn_TYPE_member_t asn_MBR_SymmetricEncryptionKey_1[2];

#ifdef __cplusplus
}
#endif

#endif	/* _SymmetricEncryptionKey_H_ */
#include <asn_internal.h>
