/*
 * Generated by asn1c-0.9.29-DF (http://lionet.info/asn1c)
 * From ASN.1 module "Ieee1609Dot2Dot1Acpc"
 * 	found in "asn1/ieee1609.2.1/Ieee1609Dot2Dot1Acpc.asn"
 * 	`asn1c -S ../../../../asn1c-fillabs2/skeletons -fcompound-names -no-gen-BER -no-gen-JER -no-gen-UPER -no-gen-APER -no-gen-random-fill -no-gen-example -pdu=EtsiTs103097Data -pdu=EnrolmentRequestMessage -pdu=EnrolmentResponseMessage -pdu=AuthorizationRequestMessage -pdu=AuthorizationRequestMessageWithPop -pdu=AuthorizationResponseMessage -pdu=CertificateRevocationListMessage -pdu=TlmCertificateTrustListMessage -pdu=RcaCertificateTrustListMessage -pdu=AuthorizationValidationRequestMessage -pdu=AuthorizationValidationResponseMessage -pdu=CaCertificateRequestMessage -pdu=EtsiTs103097Certificate`
 */


/* Including external dependencies */
#include "Ieee1609Dot2Data-Signed.h"
#ifndef	_SignedAprvBinaryTree_H_
#define	_SignedAprvBinaryTree_H_


#include <asn_application.h>

#ifdef __cplusplus
extern "C" {
#endif

/* SignedAprvBinaryTree */
typedef Ieee1609Dot2Data_Signed_228P1_t	 SignedAprvBinaryTree_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_SignedAprvBinaryTree;
asn_struct_free_f SignedAprvBinaryTree_free;
asn_struct_print_f SignedAprvBinaryTree_print;
asn_constr_check_f SignedAprvBinaryTree_constraint;
xer_type_decoder_f SignedAprvBinaryTree_decode_xer;
xer_type_encoder_f SignedAprvBinaryTree_encode_xer;
oer_type_decoder_f SignedAprvBinaryTree_decode_oer;
oer_type_encoder_f SignedAprvBinaryTree_encode_oer;

#ifdef __cplusplus
}
#endif

#endif	/* _SignedAprvBinaryTree_H_ */
#include <asn_internal.h>
