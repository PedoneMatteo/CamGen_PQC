/*
 * Generated by asn1c-0.9.29-DF (http://lionet.info/asn1c)
 * From ASN.1 module "Ieee1609Dot2"
 * 	found in "asn1/ieee1609.2/Ieee1609Dot2.asn"
 * 	`asn1c -fcompound-names -no-gen-BER -no-gen-JER -no-gen-UPER -no-gen-APER -no-gen-random-fill -no-gen-example -pdu=EtsiTs103097Data -pdu=EnrolmentRequestMessage -pdu=EnrolmentResponseMessage -pdu=AuthorizationRequestMessage -pdu=AuthorizationRequestMessageWithPop -pdu=AuthorizationResponseMessage -pdu=CertificateRevocationListMessage -pdu=TlmCertificateTrustListMessage -pdu=RcaCertificateTrustListMessage -pdu=AuthorizationValidationRequestMessage -pdu=AuthorizationValidationResponseMessage -pdu=CaCertificateRequestMessage -pdu=EtsiTs103097Certificate`
 */


/* Including external dependencies */
#include "SequenceOfPsidSspRange.h"
#include <NULL.h>
#include <constr_CHOICE.h>
#ifndef	_SubjectPermissions_H_
#define	_SubjectPermissions_H_


#include <asn_application.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum SubjectPermissions_PR {
	SubjectPermissions_PR_NOTHING,	/* No components present */
	SubjectPermissions_PR_explicit,
	SubjectPermissions_PR_all
	/* Extensions may appear below */
	
} SubjectPermissions_PR;

/* SubjectPermissions */
typedef struct SubjectPermissions {
	SubjectPermissions_PR present;
	union SubjectPermissions_u {
		SequenceOfPsidSspRange_t	 Explicit;
		NULL_t	 all;
		/*
		 * This type is extensible,
		 * possible extensions are below.
		 */
	} choice;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} SubjectPermissions_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_SubjectPermissions;
extern asn_CHOICE_specifics_t asn_SPC_SubjectPermissions_specs_1;
extern asn_TYPE_member_t asn_MBR_SubjectPermissions_1[2];

#ifdef __cplusplus
}
#endif

#endif	/* _SubjectPermissions_H_ */
#include <asn_internal.h>
