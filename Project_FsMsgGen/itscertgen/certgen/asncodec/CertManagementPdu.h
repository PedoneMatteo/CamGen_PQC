/*
 * Generated by asn1c-0.9.29-DF (http://lionet.info/asn1c)
 * From ASN.1 module "Ieee1609Dot2Dot1CertManagement"
 * 	found in "asn1/ieee1609.2.1/Ieee1609Dot2Dot1CertManagement.asn"
 * 	`asn1c -fcompound-names -no-gen-BER -no-gen-JER -no-gen-UPER -no-gen-APER -no-gen-random-fill -no-gen-example -pdu=EtsiTs103097Data -pdu=EnrolmentRequestMessage -pdu=EnrolmentResponseMessage -pdu=AuthorizationRequestMessage -pdu=AuthorizationRequestMessageWithPop -pdu=AuthorizationResponseMessage -pdu=CertificateRevocationListMessage -pdu=TlmCertificateTrustListMessage -pdu=RcaCertificateTrustListMessage -pdu=AuthorizationValidationRequestMessage -pdu=AuthorizationValidationResponseMessage -pdu=CaCertificateRequestMessage -pdu=EtsiTs103097Certificate`
 */


/* Including external dependencies */
#include "CompositeCrl.h"
#include "CertificateChain.h"
#include "MultiSignedCtl.h"
#include "ToBeSignedCtlSignature.h"
#include "CertificateManagementInfoStatus.h"
#include <constr_CHOICE.h>
#ifndef	_CertManagementPdu_H_
#define	_CertManagementPdu_H_


#include <asn_application.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum CertManagementPdu_PR {
	CertManagementPdu_PR_NOTHING,	/* No components present */
	CertManagementPdu_PR_compositeCrl,
	CertManagementPdu_PR_certificateChain,
	CertManagementPdu_PR_multiSignedCtl,
	CertManagementPdu_PR_tbsCtlSignature,
	CertManagementPdu_PR_infoStatus
	/* Extensions may appear below */
	
} CertManagementPdu_PR;

/* CertManagementPdu */
typedef struct CertManagementPdu {
	CertManagementPdu_PR present;
	union CertManagementPdu_u {
		CompositeCrl_t	 compositeCrl;
		CertificateChain_t	 certificateChain;
		MultiSignedCtl_t	 multiSignedCtl;
		ToBeSignedCtlSignature_t	 tbsCtlSignature;
		CertificateManagementInfoStatus_t	 infoStatus;
		/*
		 * This type is extensible,
		 * possible extensions are below.
		 */
	} choice;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} CertManagementPdu_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_CertManagementPdu;
extern asn_CHOICE_specifics_t asn_SPC_CertManagementPdu_specs_1;
extern asn_TYPE_member_t asn_MBR_CertManagementPdu_1[5];

#ifdef __cplusplus
}
#endif

#endif	/* _CertManagementPdu_H_ */
#include <asn_internal.h>
