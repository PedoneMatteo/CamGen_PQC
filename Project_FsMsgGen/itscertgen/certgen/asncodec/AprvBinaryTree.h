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
#include "HashAlgorithm.h"
#include <BIT_STRING.h>
#include "AcpcNodeValue.h"
#include <asn_SEQUENCE_OF.h>
#include <constr_SEQUENCE_OF.h>
#include <constr_SEQUENCE.h>
#ifndef	_AprvBinaryTree_H_
#define	_AprvBinaryTree_H_


#include <asn_application.h>

#ifdef __cplusplus
extern "C" {
#endif

/* AprvBinaryTree */
typedef struct AprvBinaryTree {
	Uint8_t	 version;
	Time32_t	 generationTime;
	IValue_t	 currentI;
	AcpcTreeId_t	 acpcTreeId;
	HashAlgorithm_t	 hashAlgorithmId;
	BIT_STRING_t	 tree;
	struct AprvBinaryTree__nodeValueList {
		A_SEQUENCE_OF(AcpcNodeValue_t) list;
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} nodeValueList;
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} AprvBinaryTree_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_AprvBinaryTree;
extern asn_SEQUENCE_specifics_t asn_SPC_AprvBinaryTree_specs_1;
extern asn_TYPE_member_t asn_MBR_AprvBinaryTree_1[7];

#ifdef __cplusplus
}
#endif

#endif	/* _AprvBinaryTree_H_ */
#include <asn_internal.h>
