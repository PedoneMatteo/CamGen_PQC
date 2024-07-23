/*
 * Generated by asn1c-0.9.29-DF (http://lionet.info/asn1c)
 * From ASN.1 module "Ieee1609Dot2"
 * 	found in "asn1/ieee1609.2/IEEE1609dot2.asn"
 * 	`asn1c -S ../../../../asn1c-fillabs2/skeletons -fcompound-names -no-gen-BER -no-gen-JER -no-gen-UPER -no-gen-APER -no-gen-random-fill -no-gen-example -pdu=EtsiTs103097Data -pdu=EnrolmentRequestMessage -pdu=EnrolmentResponseMessage -pdu=AuthorizationRequestMessage -pdu=AuthorizationRequestMessageWithPop -pdu=AuthorizationResponseMessage -pdu=CertificateRevocationListMessage -pdu=TlmCertificateTrustListMessage -pdu=RcaCertificateTrustListMessage -pdu=AuthorizationValidationRequestMessage -pdu=AuthorizationValidationResponseMessage -pdu=CaCertificateRequestMessage -pdu=EtsiTs103097Certificate`
 */


/* Including external dependencies */
#include "LinkageData.h"
#include "Hostname.h"
#include <OCTET_STRING.h>
#include <NULL.h>
#include <constr_CHOICE.h>
#ifndef	_CertificateId_H_
#define	_CertificateId_H_


#include <asn_application.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum CertificateId_PR {
	CertificateId_PR_NOTHING,	/* No components present */
	CertificateId_PR_linkageData,
	CertificateId_PR_name,
	CertificateId_PR_binaryId,
	CertificateId_PR_none
	/* Extensions may appear below */
	
} CertificateId_PR;

/* CertificateId */
typedef struct CertificateId {
	CertificateId_PR present;
	union CertificateId_u {
		LinkageData_t	 linkageData;
		Hostname_t	 name;
		OCTET_STRING_t	 binaryId;
		NULL_t	 none;
		/*
		 * This type is extensible,
		 * possible extensions are below.
		 */
	} choice;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} CertificateId_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_CertificateId;
extern asn_CHOICE_specifics_t asn_SPC_CertificateId_specs_1;
extern asn_TYPE_member_t asn_MBR_CertificateId_1[4];

#ifdef __cplusplus
}
#endif

#endif	/* _CertificateId_H_ */
#include <asn_internal.h>