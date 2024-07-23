/*
 * Generated by asn1c-0.9.29-DF (http://lionet.info/asn1c)
 * From ASN.1 module "Ieee1609Dot2"
 * 	found in "asn1/ieee1609.2/IEEE1609dot2.asn"
 * 	`asn1c -S ../../../../asn1c-fillabs2/skeletons -fcompound-names -no-gen-BER -no-gen-JER -no-gen-UPER -no-gen-APER -no-gen-random-fill -no-gen-example -pdu=EtsiTs103097Data -pdu=EnrolmentRequestMessage -pdu=EnrolmentResponseMessage -pdu=AuthorizationRequestMessage -pdu=AuthorizationRequestMessageWithPop -pdu=AuthorizationResponseMessage -pdu=CertificateRevocationListMessage -pdu=TlmCertificateTrustListMessage -pdu=RcaCertificateTrustListMessage -pdu=AuthorizationValidationRequestMessage -pdu=AuthorizationValidationResponseMessage -pdu=CaCertificateRequestMessage -pdu=EtsiTs103097Certificate`
 */


/* Including external dependencies */
#include <OCTET_STRING.h>
#include "Opaque.h"
#include <constr_SEQUENCE.h>
#ifndef	_One28BitCcmCiphertext_H_
#define	_One28BitCcmCiphertext_H_


#include <asn_application.h>

#ifdef __cplusplus
extern "C" {
#endif

/* One28BitCcmCiphertext */
typedef struct One28BitCcmCiphertext {
	OCTET_STRING_t	 nonce;
	Opaque_t	 ccmCiphertext;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} One28BitCcmCiphertext_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_One28BitCcmCiphertext;
extern asn_SEQUENCE_specifics_t asn_SPC_One28BitCcmCiphertext_specs_1;
extern asn_TYPE_member_t asn_MBR_One28BitCcmCiphertext_1[2];

#ifdef __cplusplus
}
#endif

#endif	/* _One28BitCcmCiphertext_H_ */
#include <asn_internal.h>