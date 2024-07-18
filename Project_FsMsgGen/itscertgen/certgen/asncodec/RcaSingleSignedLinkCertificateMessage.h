/*
 * Generated by asn1c-0.9.29-DF (http://lionet.info/asn1c)
 * From ASN.1 module "EtsiTs102941MessagesCa"
 * 	found in "asn1/pki_ts102941/EtsiTs102941MessagesCA.asn"
 * 	`asn1c -S ../../../../asn1c-fillabs2/skeletons -fcompound-names -no-gen-BER -no-gen-JER -no-gen-UPER -no-gen-APER -no-gen-random-fill -no-gen-example -pdu=EtsiTs103097Data -pdu=EnrolmentRequestMessage -pdu=EnrolmentResponseMessage -pdu=AuthorizationRequestMessage -pdu=AuthorizationRequestMessageWithPop -pdu=AuthorizationResponseMessage -pdu=CertificateRevocationListMessage -pdu=TlmCertificateTrustListMessage -pdu=RcaCertificateTrustListMessage -pdu=AuthorizationValidationRequestMessage -pdu=AuthorizationValidationResponseMessage -pdu=CaCertificateRequestMessage -pdu=EtsiTs103097Certificate`
 */


/* Including external dependencies */
#include "EtsiTs103097Data-Signed.h"
#ifndef	_RcaSingleSignedLinkCertificateMessage_H_
#define	_RcaSingleSignedLinkCertificateMessage_H_


#include <asn_application.h>

#ifdef __cplusplus
extern "C" {
#endif

/* RcaSingleSignedLinkCertificateMessage */
typedef EtsiTs103097Data_Signed_63P0_t	 RcaSingleSignedLinkCertificateMessage_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_RcaSingleSignedLinkCertificateMessage;
asn_struct_free_f RcaSingleSignedLinkCertificateMessage_free;
asn_struct_print_f RcaSingleSignedLinkCertificateMessage_print;
asn_constr_check_f RcaSingleSignedLinkCertificateMessage_constraint;
xer_type_decoder_f RcaSingleSignedLinkCertificateMessage_decode_xer;
xer_type_encoder_f RcaSingleSignedLinkCertificateMessage_encode_xer;
oer_type_decoder_f RcaSingleSignedLinkCertificateMessage_decode_oer;
oer_type_encoder_f RcaSingleSignedLinkCertificateMessage_encode_oer;

#ifdef __cplusplus
}
#endif

#endif	/* _RcaSingleSignedLinkCertificateMessage_H_ */
#include <asn_internal.h>
