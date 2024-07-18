/*
 * Generated by asn1c-0.9.29-DF (http://lionet.info/asn1c)
 * From ASN.1 module "EtsiTs102941MessagesCa"
 * 	found in "asn1/pki_ts102941/EtsiTs102941MessagesCA.asn"
 * 	`asn1c -S ../../../../asn1c-fillabs2/skeletons -fcompound-names -no-gen-BER -no-gen-JER -no-gen-UPER -no-gen-APER -no-gen-random-fill -no-gen-example -pdu=EtsiTs103097Data -pdu=EnrolmentRequestMessage -pdu=EnrolmentResponseMessage -pdu=AuthorizationRequestMessage -pdu=AuthorizationRequestMessageWithPop -pdu=AuthorizationResponseMessage -pdu=CertificateRevocationListMessage -pdu=TlmCertificateTrustListMessage -pdu=RcaCertificateTrustListMessage -pdu=AuthorizationValidationRequestMessage -pdu=AuthorizationValidationResponseMessage -pdu=CaCertificateRequestMessage -pdu=EtsiTs103097Certificate`
 */


/* Including external dependencies */
#include "EtsiTs103097Data-Signed.h"
#ifndef	_RcaCertificateTrustListMessage_H_
#define	_RcaCertificateTrustListMessage_H_


#include <asn_application.h>

#ifdef __cplusplus
extern "C" {
#endif

/* RcaCertificateTrustListMessage */
typedef EtsiTs103097Data_Signed_63P0_t	 RcaCertificateTrustListMessage_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_RcaCertificateTrustListMessage;
asn_struct_free_f RcaCertificateTrustListMessage_free;
asn_struct_print_f RcaCertificateTrustListMessage_print;
asn_constr_check_f RcaCertificateTrustListMessage_constraint;
xer_type_decoder_f RcaCertificateTrustListMessage_decode_xer;
xer_type_encoder_f RcaCertificateTrustListMessage_encode_xer;
oer_type_decoder_f RcaCertificateTrustListMessage_decode_oer;
oer_type_encoder_f RcaCertificateTrustListMessage_encode_oer;

#ifdef __cplusplus
}
#endif

#endif	/* _RcaCertificateTrustListMessage_H_ */
#include <asn_internal.h>
