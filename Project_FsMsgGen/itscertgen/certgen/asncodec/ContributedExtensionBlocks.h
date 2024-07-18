/*
 * Generated by asn1c-0.9.29-DF (http://lionet.info/asn1c)
 * From ASN.1 module "Ieee1609Dot2"
 * 	found in "asn1/ieee1609.2/IEEE1609dot2.asn"
 * 	`asn1c -S ../../../../asn1c-fillabs2/skeletons -fcompound-names -no-gen-BER -no-gen-JER -no-gen-UPER -no-gen-APER -no-gen-random-fill -no-gen-example -pdu=EtsiTs103097Data -pdu=EnrolmentRequestMessage -pdu=EnrolmentResponseMessage -pdu=AuthorizationRequestMessage -pdu=AuthorizationRequestMessageWithPop -pdu=AuthorizationResponseMessage -pdu=CertificateRevocationListMessage -pdu=TlmCertificateTrustListMessage -pdu=RcaCertificateTrustListMessage -pdu=AuthorizationValidationRequestMessage -pdu=AuthorizationValidationResponseMessage -pdu=CaCertificateRequestMessage -pdu=EtsiTs103097Certificate`
 */


/* Including external dependencies */
#include <asn_SEQUENCE_OF.h>
#include <constr_SEQUENCE_OF.h>
#ifndef	_ContributedExtensionBlocks_H_
#define	_ContributedExtensionBlocks_H_


#include <asn_application.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Forward declarations */
struct ContributedExtensionBlock;

/* ContributedExtensionBlocks */
typedef struct ContributedExtensionBlocks {
	A_SEQUENCE_OF(struct ContributedExtensionBlock) list;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} ContributedExtensionBlocks_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_ContributedExtensionBlocks;
extern asn_SET_OF_specifics_t asn_SPC_ContributedExtensionBlocks_specs_1;
extern asn_TYPE_member_t asn_MBR_ContributedExtensionBlocks_1[1];

#ifdef __cplusplus
}
#endif

/* Referred external types */
#include "ContributedExtensionBlock.h"

#endif	/* _ContributedExtensionBlocks_H_ */
#include <asn_internal.h>
