/*
 * Generated by asn1c-0.9.29-DF (http://lionet.info/asn1c)
 * From ASN.1 module "EtsiTs102941MessagesCa"
 * 	found in "asn1/pki_ts102941/EtsiTs102941MessagesCa.asn"
 * 	`asn1c -fcompound-names -no-gen-BER -no-gen-JER -no-gen-UPER -no-gen-APER -no-gen-random-fill -no-gen-example -pdu=EtsiTs103097Data -pdu=EnrolmentRequestMessage -pdu=EnrolmentResponseMessage -pdu=AuthorizationRequestMessage -pdu=AuthorizationRequestMessageWithPop -pdu=AuthorizationResponseMessage -pdu=CertificateRevocationListMessage -pdu=TlmCertificateTrustListMessage -pdu=RcaCertificateTrustListMessage -pdu=AuthorizationValidationRequestMessage -pdu=AuthorizationValidationResponseMessage -pdu=CaCertificateRequestMessage -pdu=EtsiTs103097Certificate`
 */


/* Including external dependencies */
#include "Version.h"
#include "EtsiTs102941DataContent.h"
#include <constr_SEQUENCE.h>
#ifndef	_EtsiTs102941Data_H_
#define	_EtsiTs102941Data_H_


#include <asn_application.h>

#ifdef __cplusplus
extern "C" {
#endif

/* EtsiTs102941Data */
typedef struct EtsiTs102941Data {
	Version_t	 version;
	EtsiTs102941DataContent_t	 content;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} EtsiTs102941Data_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_EtsiTs102941Data;

#ifdef __cplusplus
}
#endif

#endif	/* _EtsiTs102941Data_H_ */
#include <asn_internal.h>
