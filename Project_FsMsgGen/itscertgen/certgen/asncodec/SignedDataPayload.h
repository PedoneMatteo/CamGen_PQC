/*
 * Generated by asn1c-0.9.29-DF (http://lionet.info/asn1c)
 * From ASN.1 module "Ieee1609Dot2"
 * 	found in "asn1/ieee1609.2/Ieee1609Dot2.asn"
 * 	`asn1c -fcompound-names -no-gen-BER -no-gen-JER -no-gen-UPER -no-gen-APER -no-gen-random-fill -no-gen-example -pdu=EtsiTs103097Data -pdu=EnrolmentRequestMessage -pdu=EnrolmentResponseMessage -pdu=AuthorizationRequestMessage -pdu=AuthorizationRequestMessageWithPop -pdu=AuthorizationResponseMessage -pdu=CertificateRevocationListMessage -pdu=TlmCertificateTrustListMessage -pdu=RcaCertificateTrustListMessage -pdu=AuthorizationValidationRequestMessage -pdu=AuthorizationValidationResponseMessage -pdu=CaCertificateRequestMessage -pdu=EtsiTs103097Certificate`
 */


/* Including external dependencies */
#include <NULL.h>
#include <constr_SEQUENCE.h>
#ifndef	_SignedDataPayload_H_
#define	_SignedDataPayload_H_


#include <asn_application.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Forward declarations */
struct Ieee1609Dot2Data;
struct HashedData;

/* SignedDataPayload */
typedef struct SignedDataPayload {
	struct Ieee1609Dot2Data	*data;	/* OPTIONAL */
	struct HashedData	*extDataHash;	/* OPTIONAL */
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	NULL_t	*omitted;	/* OPTIONAL */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} SignedDataPayload_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_SignedDataPayload;
extern asn_SEQUENCE_specifics_t asn_SPC_SignedDataPayload_specs_1;
extern asn_TYPE_member_t asn_MBR_SignedDataPayload_1[3];

#ifdef __cplusplus
}
#endif

/* Referred external types */
#include "Ieee1609Dot2Data.h"
#include "HashedData.h"

#endif	/* _SignedDataPayload_H_ */
#include <asn_internal.h>
