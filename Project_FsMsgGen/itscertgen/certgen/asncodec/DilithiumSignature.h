/*
 * Generated by asn1c-0.9.29-DF (http://lionet.info/asn1c)
 * From ASN.1 module "Ieee1609Dot2BaseTypes"
 * 	found in "asn1/ieee1609.2/Ieee1609Dot2BaseTypes.asn"
 * 	`asn1c -fcompound-names -no-gen-BER -no-gen-JER -no-gen-UPER -no-gen-APER -no-gen-random-fill -no-gen-example -pdu=EtsiTs103097Data -pdu=EnrolmentRequestMessage -pdu=EnrolmentResponseMessage -pdu=AuthorizationRequestMessage -pdu=AuthorizationRequestMessageWithPop -pdu=AuthorizationResponseMessage -pdu=CertificateRevocationListMessage -pdu=TlmCertificateTrustListMessage -pdu=RcaCertificateTrustListMessage -pdu=AuthorizationValidationRequestMessage -pdu=AuthorizationValidationResponseMessage -pdu=CaCertificateRequestMessage -pdu=EtsiTs103097Certificate`
 */


/* Including external dependencies */
#include "Uint8.h"
#include "Uint16.h"
#include <OCTET_STRING.h>
#include <constr_SEQUENCE.h>
#ifndef	_DilithiumSignature_H_
#define	_DilithiumSignature_H_


#include <asn_application.h>

#ifdef __cplusplus
extern "C" {
#endif

/* DilithiumSignature */
typedef struct DilithiumSignature {
	Uint8_t	 algVersion;
	Uint16_t	 signatureLen;
	OCTET_STRING_t	 signatureValue;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} DilithiumSignature_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_DilithiumSignature;
extern asn_SEQUENCE_specifics_t asn_SPC_DilithiumSignature_specs_1;
extern asn_TYPE_member_t asn_MBR_DilithiumSignature_1[3];

#ifdef __cplusplus
}
#endif

#endif	/* _DilithiumSignature_H_ */
#include <asn_internal.h>
