/*
 * Generated by asn1c-0.9.29-DF (http://lionet.info/asn1c)
 * From ASN.1 module "Ieee1609Dot2BaseTypes"
 * 	found in "asn1/ieee1609.2/IEEE1609dot2BaseTypes.asn"
 * 	`asn1c -S ../../../../asn1c-fillabs2/skeletons -fcompound-names -no-gen-BER -no-gen-JER -no-gen-UPER -no-gen-APER -no-gen-random-fill -no-gen-example -pdu=EtsiTs103097Data -pdu=EnrolmentRequestMessage -pdu=EnrolmentResponseMessage -pdu=AuthorizationRequestMessage -pdu=AuthorizationRequestMessageWithPop -pdu=AuthorizationResponseMessage -pdu=CertificateRevocationListMessage -pdu=TlmCertificateTrustListMessage -pdu=RcaCertificateTrustListMessage -pdu=AuthorizationValidationRequestMessage -pdu=AuthorizationValidationResponseMessage -pdu=CaCertificateRequestMessage -pdu=EtsiTs103097Certificate`
 */


/* Including external dependencies */
#include "EccP256CurvePoint.h"
#include <constr_CHOICE.h>
#ifndef	_BasePublicEncryptionKey_H_
#define	_BasePublicEncryptionKey_H_


#include <asn_application.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum BasePublicEncryptionKey_PR {
	BasePublicEncryptionKey_PR_NOTHING,	/* No components present */
	BasePublicEncryptionKey_PR_eciesNistP256,
	BasePublicEncryptionKey_PR_eciesBrainpoolP256r1,
	/* Extensions may appear below */
	BasePublicEncryptionKey_PR_ecencSm2
} BasePublicEncryptionKey_PR;

/* BasePublicEncryptionKey */
typedef struct BasePublicEncryptionKey {
	BasePublicEncryptionKey_PR present;
	union BasePublicEncryptionKey_u {
		EccP256CurvePoint_t	 eciesNistP256;
		EccP256CurvePoint_t	 eciesBrainpoolP256r1;
		/*
		 * This type is extensible,
		 * possible extensions are below.
		 */
		EccP256CurvePoint_t	 ecencSm2;
	} choice;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} BasePublicEncryptionKey_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_BasePublicEncryptionKey;
extern asn_CHOICE_specifics_t asn_SPC_BasePublicEncryptionKey_specs_1;
extern asn_TYPE_member_t asn_MBR_BasePublicEncryptionKey_1[3];

#ifdef __cplusplus
}
#endif

#endif	/* _BasePublicEncryptionKey_H_ */
#include <asn_internal.h>
