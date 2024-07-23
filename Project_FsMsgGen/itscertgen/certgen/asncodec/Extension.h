/*
 * Generated by asn1c-0.9.29-DF (http://lionet.info/asn1c)
 * From ASN.1 module "Ieee1609Dot2BaseTypes"
 * 	found in "asn1/ieee1609.2/IEEE1609dot2BaseTypes.asn"
 * 	`asn1c -S ../../../../asn1c-fillabs2/skeletons -fcompound-names -no-gen-BER -no-gen-JER -no-gen-UPER -no-gen-APER -no-gen-random-fill -no-gen-example -pdu=EtsiTs103097Data -pdu=EnrolmentRequestMessage -pdu=EnrolmentResponseMessage -pdu=AuthorizationRequestMessage -pdu=AuthorizationRequestMessageWithPop -pdu=AuthorizationResponseMessage -pdu=CertificateRevocationListMessage -pdu=TlmCertificateTrustListMessage -pdu=RcaCertificateTrustListMessage -pdu=AuthorizationValidationRequestMessage -pdu=AuthorizationValidationResponseMessage -pdu=CaCertificateRequestMessage -pdu=EtsiTs103097Certificate`
 */


/* Including external dependencies */
#include "ExtId.h"
#include <ANY.h>
#include <asn_ioc.h>
#include "EtsiTs102941CrlRequest.h"
#include "EtsiTs102941DeltaCtlRequest.h"
#include <OPEN_TYPE.h>
#include <constr_CHOICE.h>
#include <constr_SEQUENCE.h>
#include "HashedId8.h"
#ifndef	_Extension_H_
#define	_Extension_H_


#include <asn_application.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum EtsiTs103097HeaderInfoExtensions__content_PR {
	EtsiTs103097HeaderInfoExtensions__content_PR_NOTHING,	/* No components present */
	EtsiTs103097HeaderInfoExtensions__content_PR_EtsiTs102941CrlRequest,
	EtsiTs103097HeaderInfoExtensions__content_PR_EtsiTs102941DeltaCtlRequest
} EtsiTs103097HeaderInfoExtensions__content_PR;
typedef enum Ieee1609HeaderInfoExtensions__content_PR {
	Ieee1609HeaderInfoExtensions__content_PR_NOTHING,	/* No components present */
	Ieee1609HeaderInfoExtensions__content_PR_HashedId8
} Ieee1609HeaderInfoExtensions__content_PR;

/* Extension */
typedef struct EtsiTs103097HeaderInfoExtensions {
	ExtId_t	 id;
	struct EtsiTs103097HeaderInfoExtensions__content {
		EtsiTs103097HeaderInfoExtensions__content_PR present;
		union EtsiTs103097HeaderInfoExtensions__content_u {
			EtsiTs102941CrlRequest_t	 EtsiTs102941CrlRequest;
			EtsiTs102941DeltaCtlRequest_t	 EtsiTs102941DeltaCtlRequest;
		} choice;
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} content;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} EtsiTs103097HeaderInfoExtensions_t;
typedef struct Ieee1609HeaderInfoExtensions {
	ExtId_t	 id;
	struct Ieee1609HeaderInfoExtensions__content {
		Ieee1609HeaderInfoExtensions__content_PR present;
		union Ieee1609HeaderInfoExtensions__content_u {
			HashedId8_t	 HashedId8;
		} choice;
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} content;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} Ieee1609HeaderInfoExtensions_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_EtsiTs103097HeaderInfoExtensions;
extern asn_SEQUENCE_specifics_t asn_SPC_EtsiTs103097HeaderInfoExtensions_specs_1;
extern asn_TYPE_member_t asn_MBR_EtsiTs103097HeaderInfoExtensions_1[2];
extern asn_TYPE_descriptor_t asn_DEF_Ieee1609HeaderInfoExtensions;
extern asn_SEQUENCE_specifics_t asn_SPC_Ieee1609HeaderInfoExtensions_specs_4;
extern asn_TYPE_member_t asn_MBR_Ieee1609HeaderInfoExtensions_4[2];

#ifdef __cplusplus
}
#endif

#endif	/* _Extension_H_ */
#include <asn_internal.h>