/*
 * Generated by asn1c-0.9.29-DF (http://lionet.info/asn1c)
 * From ASN.1 module "Ieee1609Dot2Dot1Protocol"
 * 	found in "asn1/ieee1609.2.1/Ieee1609Dot2Dot1Protocol.asn"
 * 	`asn1c -S ../../../../asn1c-fillabs2/skeletons -fcompound-names -no-gen-BER -no-gen-JER -no-gen-UPER -no-gen-APER -no-gen-random-fill -no-gen-example -pdu=EtsiTs103097Data -pdu=EnrolmentRequestMessage -pdu=EnrolmentResponseMessage -pdu=AuthorizationRequestMessage -pdu=AuthorizationRequestMessageWithPop -pdu=AuthorizationResponseMessage -pdu=CertificateRevocationListMessage -pdu=TlmCertificateTrustListMessage -pdu=RcaCertificateTrustListMessage -pdu=AuthorizationValidationRequestMessage -pdu=AuthorizationValidationResponseMessage -pdu=CaCertificateRequestMessage -pdu=EtsiTs103097Certificate`
 */


/* Including external dependencies */
#include "Uint8.h"
#include <constr_SEQUENCE.h>
#ifndef	_LopSsp_H_
#define	_LopSsp_H_


#include <asn_application.h>

#ifdef __cplusplus
extern "C" {
#endif

/* LopSsp */
typedef struct LopSsp {
	Uint8_t	 version;
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} LopSsp_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_LopSsp;
extern asn_SEQUENCE_specifics_t asn_SPC_LopSsp_specs_1;
extern asn_TYPE_member_t asn_MBR_LopSsp_1[1];

#ifdef __cplusplus
}
#endif

#endif	/* _LopSsp_H_ */
#include <asn_internal.h>
