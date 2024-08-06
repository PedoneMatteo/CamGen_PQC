/*
 * Generated by asn1c-0.9.29-DF (http://lionet.info/asn1c)
 * From ASN.1 module "Ieee1609Dot2Dot1EeRaInterface"
 * 	found in "asn1/ieee1609.2.1/Ieee1609Dot2Dot1EeRaInterface.asn"
 * 	`asn1c -fcompound-names -no-gen-BER -no-gen-JER -no-gen-UPER -no-gen-APER -no-gen-random-fill -no-gen-example -pdu=EtsiTs103097Data -pdu=EnrolmentRequestMessage -pdu=EnrolmentResponseMessage -pdu=AuthorizationRequestMessage -pdu=AuthorizationRequestMessageWithPop -pdu=AuthorizationResponseMessage -pdu=CertificateRevocationListMessage -pdu=TlmCertificateTrustListMessage -pdu=RcaCertificateTrustListMessage -pdu=AuthorizationValidationRequestMessage -pdu=AuthorizationValidationResponseMessage -pdu=CaCertificateRequestMessage -pdu=EtsiTs103097Certificate`
 */


/* Including external dependencies */
#include "Uint8.h"
#include "Time32.h"
#include "CertificateType.h"
#include "ToBeSignedCertificate.h"
#include <constr_SEQUENCE.h>
#ifndef	_EeRaCertRequest_H_
#define	_EeRaCertRequest_H_


#include <asn_application.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Forward declarations */
struct AdditionalParams;

/* EeRaCertRequest */
typedef struct EeRaCertRequest {
	Uint8_t	 version;
	Time32_t	 generationTime;
	CertificateType_t	 type;
	ToBeSignedCertificate_t	 tbsCert;
	struct AdditionalParams	*additionalParams;	/* OPTIONAL */
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} EeRaCertRequest_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_EeRaCertRequest;
extern asn_SEQUENCE_specifics_t asn_SPC_EeRaCertRequest_specs_1;
extern asn_TYPE_member_t asn_MBR_EeRaCertRequest_1[5];

#ifdef __cplusplus
}
#endif

/* Referred external types */
#include "AdditionalParams.h"

#endif	/* _EeRaCertRequest_H_ */
#include <asn_internal.h>
