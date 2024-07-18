/*
 * Generated by asn1c-0.9.29-DF (http://lionet.info/asn1c)
 * From ASN.1 module "EtsiTs102941MessagesCa"
 * 	found in "asn1/pki_ts102941/EtsiTs102941MessagesCA.asn"
 * 	`asn1c -S ../../../../asn1c-fillabs2/skeletons -fcompound-names -no-gen-BER -no-gen-JER -no-gen-UPER -no-gen-APER -no-gen-random-fill -no-gen-example -pdu=EtsiTs103097Data -pdu=EnrolmentRequestMessage -pdu=EnrolmentResponseMessage -pdu=AuthorizationRequestMessage -pdu=AuthorizationRequestMessageWithPop -pdu=AuthorizationResponseMessage -pdu=CertificateRevocationListMessage -pdu=TlmCertificateTrustListMessage -pdu=RcaCertificateTrustListMessage -pdu=AuthorizationValidationRequestMessage -pdu=AuthorizationValidationResponseMessage -pdu=CaCertificateRequestMessage -pdu=EtsiTs103097Certificate`
 */


/* Including external dependencies */
#include "InnerEcRequestSignedForPop.h"
#include "InnerEcResponse.h"
#include "InnerAtRequest.h"
#include "InnerAtResponse.h"
#include "ToBeSignedCrl.h"
#include "ToBeSignedTlmCtl.h"
#include "ToBeSignedRcaCtl.h"
#include "AuthorizationValidationRequest.h"
#include "AuthorizationValidationResponse.h"
#include "CaCertificateRequest.h"
#include "ToBeSignedLinkCertificateTlm.h"
#include "ToBeSignedLinkCertificateRca.h"
#include "RcaSingleSignedLinkCertificateMessage.h"
#include "EeRaCertRequest.h"
#include "EtsiTs102941ButterflyAuthorizationRequest-X509Signed.h"
#include "RaEeCertInfo.h"
#include "RaAcaCertRequest.h"
#include "AcaRaCertResponse.h"
#include "EeRaDownloadRequest.h"
#include <constr_SEQUENCE.h>
#include <constr_CHOICE.h>
#ifndef	_EtsiTs102941DataContent_H_
#define	_EtsiTs102941DataContent_H_


#include <asn_application.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum EtsiTs102941DataContent_PR {
	EtsiTs102941DataContent_PR_NOTHING,	/* No components present */
	EtsiTs102941DataContent_PR_enrolmentRequest,
	EtsiTs102941DataContent_PR_enrolmentResponse,
	EtsiTs102941DataContent_PR_authorizationRequest,
	EtsiTs102941DataContent_PR_authorizationResponse,
	EtsiTs102941DataContent_PR_certificateRevocationList,
	EtsiTs102941DataContent_PR_certificateTrustListTlm,
	EtsiTs102941DataContent_PR_certificateTrustListRca,
	EtsiTs102941DataContent_PR_authorizationValidationRequest,
	EtsiTs102941DataContent_PR_authorizationValidationResponse,
	EtsiTs102941DataContent_PR_caCertificateRequest,
	/* Extensions may appear below */
	EtsiTs102941DataContent_PR_linkCertificateTlm,
	EtsiTs102941DataContent_PR_singleSignedLinkCertificateRca,
	EtsiTs102941DataContent_PR_doubleSignedlinkCertificateRca,
	EtsiTs102941DataContent_PR_ext1
} EtsiTs102941DataContent_PR;

/* EtsiTs102941DataContent */
typedef struct EtsiTs102941DataContent {
	EtsiTs102941DataContent_PR present;
	union EtsiTs102941DataContent_u {
		InnerEcRequestSignedForPop_t	 enrolmentRequest;
		InnerEcResponse_t	 enrolmentResponse;
		InnerAtRequest_t	 authorizationRequest;
		InnerAtResponse_t	 authorizationResponse;
		ToBeSignedCrl_t	 certificateRevocationList;
		ToBeSignedTlmCtl_t	 certificateTrustListTlm;
		ToBeSignedRcaCtl_t	 certificateTrustListRca;
		AuthorizationValidationRequest_t	 authorizationValidationRequest;
		AuthorizationValidationResponse_t	 authorizationValidationResponse;
		CaCertificateRequest_t	 caCertificateRequest;
		/*
		 * This type is extensible,
		 * possible extensions are below.
		 */
		ToBeSignedLinkCertificateTlm_t	 linkCertificateTlm;
		ToBeSignedLinkCertificateRca_t	 singleSignedLinkCertificateRca;
		RcaSingleSignedLinkCertificateMessage_t	 doubleSignedlinkCertificateRca;
		struct EtsiTs102941DataContent__ext1 {
			EeRaCertRequest_t	 butterflyAuthorizationRequest;
			EtsiTs102941ButterflyAuthorizationRequest_X509Signed_t	 x509SignedbutterflyAuthorizationRequest;
			RaEeCertInfo_t	 butterflyAuthorizationResponse;
			RaAcaCertRequest_t	 butterflyCertificateRequest;
			AcaRaCertResponse_t	 butterflyCertificateResponse;
			EeRaDownloadRequest_t	 butterflyAtDownloadRequest;
			
			/* Context for parsing across buffer boundaries */
			asn_struct_ctx_t _asn_ctx;
		} *ext1;
	} choice;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} EtsiTs102941DataContent_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_EtsiTs102941DataContent;
extern asn_CHOICE_specifics_t asn_SPC_EtsiTs102941DataContent_specs_1;
extern asn_TYPE_member_t asn_MBR_EtsiTs102941DataContent_1[14];

#ifdef __cplusplus
}
#endif

#endif	/* _EtsiTs102941DataContent_H_ */
#include <asn_internal.h>
