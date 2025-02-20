/*
 * Generated by asn1c-0.9.29-DF (http://lionet.info/asn1c)
 * From ASN.1 module "Ieee1609Dot2Dot1Protocol"
 * 	found in "asn1/ieee1609.2.1/Ieee1609Dot2Dot1Protocol.asn"
 * 	`asn1c -fcompound-names -no-gen-BER -no-gen-JER -no-gen-UPER -no-gen-APER -no-gen-random-fill -no-gen-example -pdu=EtsiTs103097Data -pdu=EnrolmentRequestMessage -pdu=EnrolmentResponseMessage -pdu=AuthorizationRequestMessage -pdu=AuthorizationRequestMessageWithPop -pdu=AuthorizationResponseMessage -pdu=CertificateRevocationListMessage -pdu=TlmCertificateTrustListMessage -pdu=RcaCertificateTrustListMessage -pdu=AuthorizationValidationRequestMessage -pdu=AuthorizationValidationResponseMessage -pdu=CaCertificateRequestMessage -pdu=EtsiTs103097Certificate`
 */


/* Including external dependencies */
#include "Uint8.h"
#include "Uint16.h"
#include <constr_SEQUENCE.h>
#ifndef	_LaSsp_H_
#define	_LaSsp_H_


#include <asn_application.h>

#ifdef __cplusplus
extern "C" {
#endif

/* LaSsp */
typedef struct LaSsp {
	Uint8_t	 version;
	Uint16_t	 laId;
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} LaSsp_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_LaSsp;
extern asn_SEQUENCE_specifics_t asn_SPC_LaSsp_specs_1;
extern asn_TYPE_member_t asn_MBR_LaSsp_1[2];

#ifdef __cplusplus
}
#endif

#endif	/* _LaSsp_H_ */
#include <asn_internal.h>
