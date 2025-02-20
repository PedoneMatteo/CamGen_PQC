/*
 * Generated by asn1c-0.9.29-DF (http://lionet.info/asn1c)
 * From ASN.1 module "EtsiTs103097ExtensionModule"
 * 	found in "asn1/sec_ts103097/EtsiTs103097ExtensionModule.asn"
 * 	`asn1c -fcompound-names -no-gen-BER -no-gen-JER -no-gen-UPER -no-gen-APER -no-gen-random-fill -no-gen-example -pdu=EtsiTs103097Data -pdu=EnrolmentRequestMessage -pdu=EnrolmentResponseMessage -pdu=AuthorizationRequestMessage -pdu=AuthorizationRequestMessageWithPop -pdu=AuthorizationResponseMessage -pdu=CertificateRevocationListMessage -pdu=TlmCertificateTrustListMessage -pdu=RcaCertificateTrustListMessage -pdu=AuthorizationValidationRequestMessage -pdu=AuthorizationValidationResponseMessage -pdu=CaCertificateRequestMessage -pdu=EtsiTs103097Certificate`
 */


/* Including external dependencies */
#include "HashedId8.h"
#include "Time32.h"
#include <constr_SEQUENCE.h>
#ifndef	_EtsiTs102941CrlRequest_H_
#define	_EtsiTs102941CrlRequest_H_


#include <asn_application.h>

#ifdef __cplusplus
extern "C" {
#endif

/* EtsiTs102941CrlRequest */
typedef struct EtsiTs102941CrlRequest {
	HashedId8_t	 issuerId;
	Time32_t	*lastKnownUpdate;	/* OPTIONAL */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} EtsiTs102941CrlRequest_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_EtsiTs102941CrlRequest;
extern asn_SEQUENCE_specifics_t asn_SPC_EtsiTs102941CrlRequest_specs_1;
extern asn_TYPE_member_t asn_MBR_EtsiTs102941CrlRequest_1[2];

#ifdef __cplusplus
}
#endif

#endif	/* _EtsiTs102941CrlRequest_H_ */
#include <asn_internal.h>
