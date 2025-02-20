/*
 * Generated by asn1c-0.9.29-DF (http://lionet.info/asn1c)
 * From ASN.1 module "EtsiTs102941TrustLists"
 * 	found in "asn1/pki_ts102941/EtsiTs102941TrustLists.asn"
 * 	`asn1c -fcompound-names -no-gen-BER -no-gen-JER -no-gen-UPER -no-gen-APER -no-gen-random-fill -no-gen-example -pdu=EtsiTs103097Data -pdu=EnrolmentRequestMessage -pdu=EnrolmentResponseMessage -pdu=AuthorizationRequestMessage -pdu=AuthorizationRequestMessageWithPop -pdu=AuthorizationResponseMessage -pdu=CertificateRevocationListMessage -pdu=TlmCertificateTrustListMessage -pdu=RcaCertificateTrustListMessage -pdu=AuthorizationValidationRequestMessage -pdu=AuthorizationValidationResponseMessage -pdu=CaCertificateRequestMessage -pdu=EtsiTs103097Certificate`
 */


/* Including external dependencies */
#include "EtsiTs103097Certificate.h"
#include <constr_SEQUENCE.h>
#ifndef	_RootCaEntry_H_
#define	_RootCaEntry_H_


#include <asn_application.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Forward declarations */
struct EtsiTs103097Certificate;

/* RootCaEntry */
typedef struct RootCaEntry {
	EtsiTs103097Certificate_t	 selfsignedRootCa;
	struct EtsiTs103097Certificate	*successorTo;	/* OPTIONAL */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} RootCaEntry_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_RootCaEntry;
extern asn_SEQUENCE_specifics_t asn_SPC_RootCaEntry_specs_1;
extern asn_TYPE_member_t asn_MBR_RootCaEntry_1[2];

#ifdef __cplusplus
}
#endif

/* Referred external types */
#include "EtsiTs103097Certificate.h"

#endif	/* _RootCaEntry_H_ */
#include <asn_internal.h>
