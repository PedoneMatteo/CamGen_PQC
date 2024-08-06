/*
 * Generated by asn1c-0.9.29-DF (http://lionet.info/asn1c)
 * From ASN.1 module "Ieee1609Dot2BaseTypes"
 * 	found in "asn1/ieee1609.2/Ieee1609Dot2BaseTypes.asn"
 * 	`asn1c -fcompound-names -no-gen-BER -no-gen-JER -no-gen-UPER -no-gen-APER -no-gen-random-fill -no-gen-example -pdu=EtsiTs103097Data -pdu=EnrolmentRequestMessage -pdu=EnrolmentResponseMessage -pdu=AuthorizationRequestMessage -pdu=AuthorizationRequestMessageWithPop -pdu=AuthorizationResponseMessage -pdu=CertificateRevocationListMessage -pdu=TlmCertificateTrustListMessage -pdu=RcaCertificateTrustListMessage -pdu=AuthorizationValidationRequestMessage -pdu=AuthorizationValidationResponseMessage -pdu=CaCertificateRequestMessage -pdu=EtsiTs103097Certificate`
 */


/* Including external dependencies */
#include <OCTET_STRING.h>
#include <NULL.h>
#include <constr_SEQUENCE.h>
#include <constr_CHOICE.h>
#ifndef	_EccP256CurvePoint_H_
#define	_EccP256CurvePoint_H_


#include <asn_application.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum EccP256CurvePoint_PR {
	EccP256CurvePoint_PR_NOTHING,	/* No components present */
	EccP256CurvePoint_PR_x_only,
	EccP256CurvePoint_PR_fill,
	EccP256CurvePoint_PR_compressed_y_0,
	EccP256CurvePoint_PR_compressed_y_1,
	EccP256CurvePoint_PR_uncompressedP256
} EccP256CurvePoint_PR;

/* EccP256CurvePoint */
typedef struct EccP256CurvePoint {
	EccP256CurvePoint_PR present;
	union EccP256CurvePoint_u {
		OCTET_STRING_t	 x_only;
		NULL_t	 fill;
		OCTET_STRING_t	 compressed_y_0;
		OCTET_STRING_t	 compressed_y_1;
		struct EccP256CurvePoint__uncompressedP256 {
			OCTET_STRING_t	 x;
			OCTET_STRING_t	 y;
			
			/* Context for parsing across buffer boundaries */
			asn_struct_ctx_t _asn_ctx;
		} uncompressedP256;
	} choice;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} EccP256CurvePoint_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_EccP256CurvePoint;
extern asn_CHOICE_specifics_t asn_SPC_EccP256CurvePoint_specs_1;
extern asn_TYPE_member_t asn_MBR_EccP256CurvePoint_1[5];

#ifdef __cplusplus
}
#endif

#endif	/* _EccP256CurvePoint_H_ */
#include <asn_internal.h>
