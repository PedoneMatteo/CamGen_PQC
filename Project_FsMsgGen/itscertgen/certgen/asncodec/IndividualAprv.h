/*
 * Generated by asn1c-0.9.29-DF (http://lionet.info/asn1c)
 * From ASN.1 module "Ieee1609Dot2Dot1Acpc"
 * 	found in "asn1/ieee1609.2.1/Ieee1609Dot2Dot1Acpc.asn"
 * 	`asn1c -fcompound-names -no-gen-BER -no-gen-JER -no-gen-UPER -no-gen-APER -no-gen-random-fill -no-gen-example -pdu=EtsiTs103097Data -pdu=EnrolmentRequestMessage -pdu=EnrolmentResponseMessage -pdu=AuthorizationRequestMessage -pdu=AuthorizationRequestMessageWithPop -pdu=AuthorizationResponseMessage -pdu=CertificateRevocationListMessage -pdu=TlmCertificateTrustListMessage -pdu=RcaCertificateTrustListMessage -pdu=AuthorizationValidationRequestMessage -pdu=AuthorizationValidationResponseMessage -pdu=CaCertificateRequestMessage -pdu=EtsiTs103097Certificate`
 */


/* Including external dependencies */
#include "Uint8.h"
#include "Time32.h"
#include "IValue.h"
#include "AcpcTreeId.h"
#include <BIT_STRING.h>
#include "AcpcNodeValue.h"
#include <constr_SEQUENCE.h>
#ifndef	_IndividualAprv_H_
#define	_IndividualAprv_H_


#include <asn_application.h>

#ifdef __cplusplus
extern "C" {
#endif

/* IndividualAprv */
typedef struct IndividualAprv {
	Uint8_t	 version;
	Time32_t	 generationTime;
	IValue_t	 currentI;
	AcpcTreeId_t	 acpcTreeId;
	BIT_STRING_t	 nodeId;
	AcpcNodeValue_t	 nodeValue;
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} IndividualAprv_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_IndividualAprv;
extern asn_SEQUENCE_specifics_t asn_SPC_IndividualAprv_specs_1;
extern asn_TYPE_member_t asn_MBR_IndividualAprv_1[6];

#ifdef __cplusplus
}
#endif

#endif	/* _IndividualAprv_H_ */
#include <asn_internal.h>
