/*
 * Generated by asn1c-0.9.29-DF (http://lionet.info/asn1c)
 * From ASN.1 module "Ieee1609Dot2Dot1Protocol"
 * 	found in "asn1/ieee1609.2.1/Ieee1609Dot2Dot1Protocol.asn"
 * 	`asn1c -fcompound-names -no-gen-BER -no-gen-JER -no-gen-UPER -no-gen-APER -no-gen-random-fill -no-gen-example -pdu=EtsiTs103097Data -pdu=EnrolmentRequestMessage -pdu=EnrolmentResponseMessage -pdu=AuthorizationRequestMessage -pdu=AuthorizationRequestMessageWithPop -pdu=AuthorizationResponseMessage -pdu=CertificateRevocationListMessage -pdu=TlmCertificateTrustListMessage -pdu=RcaCertificateTrustListMessage -pdu=AuthorizationValidationRequestMessage -pdu=AuthorizationValidationResponseMessage -pdu=CaCertificateRequestMessage -pdu=EtsiTs103097Certificate`
 */


/* Including external dependencies */
#include "CamSsp.h"
#include <constr_CHOICE.h>
#ifndef	_AcpcSsp_H_
#define	_AcpcSsp_H_


#include <asn_application.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum AcpcSsp_PR {
	AcpcSsp_PR_NOTHING,	/* No components present */
	AcpcSsp_PR_cam
	/* Extensions may appear below */
	
} AcpcSsp_PR;

/* AcpcSsp */
typedef struct AcpcSsp {
	AcpcSsp_PR present;
	union AcpcSsp_u {
		CamSsp_t	 cam;
		/*
		 * This type is extensible,
		 * possible extensions are below.
		 */
	} choice;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} AcpcSsp_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_AcpcSsp;

#ifdef __cplusplus
}
#endif

#endif	/* _AcpcSsp_H_ */
#include <asn_internal.h>
