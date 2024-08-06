/*
 * Generated by asn1c-0.9.29-DF (http://lionet.info/asn1c)
 * From ASN.1 module "Ieee1609Dot2CrlBaseTypes"
 * 	found in "asn1/ieee1609.2/Ieee1609Dot2CrlBaseTypes.asn"
 * 	`asn1c -fcompound-names -no-gen-BER -no-gen-JER -no-gen-UPER -no-gen-APER -no-gen-random-fill -no-gen-example -pdu=EtsiTs103097Data -pdu=EnrolmentRequestMessage -pdu=EnrolmentResponseMessage -pdu=AuthorizationRequestMessage -pdu=AuthorizationRequestMessageWithPop -pdu=AuthorizationResponseMessage -pdu=CertificateRevocationListMessage -pdu=TlmCertificateTrustListMessage -pdu=RcaCertificateTrustListMessage -pdu=AuthorizationValidationRequestMessage -pdu=AuthorizationValidationResponseMessage -pdu=CaCertificateRequestMessage -pdu=EtsiTs103097Certificate`
 */


/* Including external dependencies */
#include "Uint8.h"
#include "CrlSeries.h"
#include "HashedId8.h"
#include "Time32.h"
#include "CrlPriorityInfo.h"
#include "TypeSpecificCrlContents.h"
#include <constr_SEQUENCE.h>
#ifndef	_CrlContents_H_
#define	_CrlContents_H_


#include <asn_application.h>

#ifdef __cplusplus
extern "C" {
#endif

/* CrlContents */
typedef struct CrlContents {
	Uint8_t	 version;
	CrlSeries_t	 crlSeries;
	HashedId8_t	 crlCraca;
	Time32_t	 issueDate;
	Time32_t	 nextCrl;
	CrlPriorityInfo_t	 priorityInfo;
	TypeSpecificCrlContents_t	 typeSpecific;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} CrlContents_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_CrlContents;

#ifdef __cplusplus
}
#endif

#endif	/* _CrlContents_H_ */
#include <asn_internal.h>
