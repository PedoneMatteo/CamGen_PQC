/*
 * Generated by asn1c-0.9.29-DF (http://lionet.info/asn1c)
 * From ASN.1 module "Ieee1609Dot2Dot1EeRaInterface"
 * 	found in "asn1/ieee1609.2.1/Ieee1609Dot2Dot1EeRaInterface.asn"
 * 	`asn1c -fcompound-names -no-gen-BER -no-gen-JER -no-gen-UPER -no-gen-APER -no-gen-random-fill -no-gen-example -pdu=EtsiTs103097Data -pdu=EnrolmentRequestMessage -pdu=EnrolmentResponseMessage -pdu=AuthorizationRequestMessage -pdu=AuthorizationRequestMessageWithPop -pdu=AuthorizationResponseMessage -pdu=CertificateRevocationListMessage -pdu=TlmCertificateTrustListMessage -pdu=RcaCertificateTrustListMessage -pdu=AuthorizationValidationRequestMessage -pdu=AuthorizationValidationResponseMessage -pdu=CaCertificateRequestMessage -pdu=EtsiTs103097Certificate`
 */


/* Including external dependencies */
#include "EeRaCertRequest.h"
#include "RaEeCertAck.h"
#include "RaEeCertInfo.h"
#include "EeRaDownloadRequest.h"
#include "EeEcaCertRequestSpdu.h"
#include <constr_CHOICE.h>
#ifndef	_EeRaInterfacePdu_H_
#define	_EeRaInterfacePdu_H_


#include <asn_application.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum EeRaInterfacePdu_PR {
	EeRaInterfacePdu_PR_NOTHING,	/* No components present */
	EeRaInterfacePdu_PR_eeRaCertRequest,
	EeRaInterfacePdu_PR_raEeCertAck,
	EeRaInterfacePdu_PR_raEeCertInfo,
	EeRaInterfacePdu_PR_eeRaDownloadRequest,
	EeRaInterfacePdu_PR_eeRaSuccessorEnrollmentCertRequest
	/* Extensions may appear below */
	
} EeRaInterfacePdu_PR;

/* EeRaInterfacePdu */
typedef struct EeRaInterfacePdu {
	EeRaInterfacePdu_PR present;
	union EeRaInterfacePdu_u {
		EeRaCertRequest_t	 eeRaCertRequest;
		RaEeCertAck_t	 raEeCertAck;
		RaEeCertInfo_t	 raEeCertInfo;
		EeRaDownloadRequest_t	 eeRaDownloadRequest;
		EeEcaCertRequestSpdu_t	 eeRaSuccessorEnrollmentCertRequest;
		/*
		 * This type is extensible,
		 * possible extensions are below.
		 */
	} choice;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} EeRaInterfacePdu_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_EeRaInterfacePdu;
extern asn_CHOICE_specifics_t asn_SPC_EeRaInterfacePdu_specs_1;
extern asn_TYPE_member_t asn_MBR_EeRaInterfacePdu_1[5];

#ifdef __cplusplus
}
#endif

#endif	/* _EeRaInterfacePdu_H_ */
#include <asn_internal.h>
