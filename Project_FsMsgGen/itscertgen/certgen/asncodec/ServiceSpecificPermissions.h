/*
 * Generated by asn1c-0.9.29-DF (http://lionet.info/asn1c)
 * From ASN.1 module "Ieee1609Dot2BaseTypes"
 * 	found in "asn1/ieee1609.2/IEEE1609dot2BaseTypes.asn"
 * 	`asn1c -S ../../../../asn1c-fillabs2/skeletons -fcompound-names -no-gen-BER -no-gen-JER -no-gen-UPER -no-gen-APER -no-gen-random-fill -no-gen-example -pdu=EtsiTs103097Data -pdu=EnrolmentRequestMessage -pdu=EnrolmentResponseMessage -pdu=AuthorizationRequestMessage -pdu=AuthorizationRequestMessageWithPop -pdu=AuthorizationResponseMessage -pdu=CertificateRevocationListMessage -pdu=TlmCertificateTrustListMessage -pdu=RcaCertificateTrustListMessage -pdu=AuthorizationValidationRequestMessage -pdu=AuthorizationValidationResponseMessage -pdu=CaCertificateRequestMessage -pdu=EtsiTs103097Certificate`
 */


/* Including external dependencies */
#include <OCTET_STRING.h>
#include "BitmapSsp.h"
#include <constr_CHOICE.h>
#ifndef	_ServiceSpecificPermissions_H_
#define	_ServiceSpecificPermissions_H_


#include <asn_application.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum ServiceSpecificPermissions_PR {
	ServiceSpecificPermissions_PR_NOTHING,	/* No components present */
	ServiceSpecificPermissions_PR_opaque,
	/* Extensions may appear below */
	ServiceSpecificPermissions_PR_bitmapSsp
} ServiceSpecificPermissions_PR;

/* ServiceSpecificPermissions */
typedef struct ServiceSpecificPermissions {
	ServiceSpecificPermissions_PR present;
	union ServiceSpecificPermissions_u {
		OCTET_STRING_t	 opaque;
		/*
		 * This type is extensible,
		 * possible extensions are below.
		 */
		BitmapSsp_t	 bitmapSsp;
	} choice;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} ServiceSpecificPermissions_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_ServiceSpecificPermissions;
extern asn_CHOICE_specifics_t asn_SPC_ServiceSpecificPermissions_specs_1;
extern asn_TYPE_member_t asn_MBR_ServiceSpecificPermissions_1[2];

#ifdef __cplusplus
}
#endif

#endif	/* _ServiceSpecificPermissions_H_ */
#include <asn_internal.h>
