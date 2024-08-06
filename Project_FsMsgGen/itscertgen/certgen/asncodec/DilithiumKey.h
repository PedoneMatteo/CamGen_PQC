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
#ifndef	_DilithiumKey_H_
#define	_DilithiumKey_H_


#include <asn_application.h>

#ifdef __cplusplus
extern "C" {
#endif

/* DilithiumKey */
typedef struct DilithiumKey {
	Uint8_t	 algVersion;
	Uint16_t	 keyLen;
	OCTET_STRING_t	 keyValue;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} DilithiumKey_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_DilithiumKey;
extern asn_SEQUENCE_specifics_t asn_SPC_DilithiumKey_specs_1;
extern asn_TYPE_member_t asn_MBR_DilithiumKey_1[3];

#ifdef __cplusplus
}
#endif

#endif	/* _DilithiumKey_H_ */
#include <asn_internal.h>
