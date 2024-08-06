/*
 * Generated by asn1c-0.9.29-DF (http://lionet.info/asn1c)
 * From ASN.1 module "Ieee1609Dot2"
 * 	found in "asn1/ieee1609.2/Ieee1609Dot2.asn"
 * 	`asn1c -fcompound-names -no-gen-BER -no-gen-JER -no-gen-UPER -no-gen-APER -no-gen-random-fill -no-gen-example -pdu=EtsiTs103097Data -pdu=EnrolmentRequestMessage -pdu=EnrolmentResponseMessage -pdu=AuthorizationRequestMessage -pdu=AuthorizationRequestMessageWithPop -pdu=AuthorizationResponseMessage -pdu=CertificateRevocationListMessage -pdu=TlmCertificateTrustListMessage -pdu=RcaCertificateTrustListMessage -pdu=AuthorizationValidationRequestMessage -pdu=AuthorizationValidationResponseMessage -pdu=CaCertificateRequestMessage -pdu=EtsiTs103097Certificate`
 */


/* Including external dependencies */
#include <asn_SEQUENCE_OF.h>
#include <constr_SEQUENCE_OF.h>
#ifndef	_SequenceOfAppExtensions_H_
#define	_SequenceOfAppExtensions_H_


#include <asn_application.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Forward declarations */
struct AppExtension;

/* SequenceOfAppExtensions */
typedef struct SequenceOfAppExtensions {
	A_SEQUENCE_OF(struct AppExtension) list;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} SequenceOfAppExtensions_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_SequenceOfAppExtensions;
extern asn_SET_OF_specifics_t asn_SPC_SequenceOfAppExtensions_specs_1;
extern asn_TYPE_member_t asn_MBR_SequenceOfAppExtensions_1[1];

#ifdef __cplusplus
}
#endif

/* Referred external types */
#include "AppExtension.h"

#endif	/* _SequenceOfAppExtensions_H_ */
#include <asn_internal.h>
