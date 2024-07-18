/*
 * Generated by asn1c-0.9.29-DF (http://lionet.info/asn1c)
 * From ASN.1 module "Ieee1609Dot2"
 * 	found in "asn1/ieee1609.2/IEEE1609dot2.asn"
 * 	`asn1c -S ../../../../asn1c-fillabs2/skeletons -fcompound-names -no-gen-BER -no-gen-JER -no-gen-UPER -no-gen-APER -no-gen-random-fill -no-gen-example -pdu=EtsiTs103097Data -pdu=EnrolmentRequestMessage -pdu=EnrolmentResponseMessage -pdu=AuthorizationRequestMessage -pdu=AuthorizationRequestMessageWithPop -pdu=AuthorizationResponseMessage -pdu=CertificateRevocationListMessage -pdu=TlmCertificateTrustListMessage -pdu=RcaCertificateTrustListMessage -pdu=AuthorizationValidationRequestMessage -pdu=AuthorizationValidationResponseMessage -pdu=CaCertificateRequestMessage -pdu=EtsiTs103097Certificate`
 */


/* Including external dependencies */
#include "HashedId8.h"
#include "HashAlgorithm.h"
#include <constr_CHOICE.h>
#ifndef	_IssuerIdentifier_H_
#define	_IssuerIdentifier_H_


#include <asn_application.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum IssuerIdentifier_PR {
	IssuerIdentifier_PR_NOTHING,	/* No components present */
	IssuerIdentifier_PR_sha256AndDigest,
	IssuerIdentifier_PR_self,
	/* Extensions may appear below */
	IssuerIdentifier_PR_sha384AndDigest,
	IssuerIdentifier_PR_sm3AndDigest
} IssuerIdentifier_PR;

/* IssuerIdentifier */
typedef struct IssuerIdentifier {
	IssuerIdentifier_PR present;
	union IssuerIdentifier_u {
		HashedId8_t	 sha256AndDigest;
		HashAlgorithm_t	 self;
		/*
		 * This type is extensible,
		 * possible extensions are below.
		 */
		HashedId8_t	 sha384AndDigest;
		HashedId8_t	 sm3AndDigest;
	} choice;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} IssuerIdentifier_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_IssuerIdentifier;
extern asn_CHOICE_specifics_t asn_SPC_IssuerIdentifier_specs_1;
extern asn_TYPE_member_t asn_MBR_IssuerIdentifier_1[4];

#ifdef __cplusplus
}
#endif

#endif	/* _IssuerIdentifier_H_ */
#include <asn_internal.h>
