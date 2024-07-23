/*
 * Generated by asn1c-0.9.29-DF (http://lionet.info/asn1c)
 * From ASN.1 module "EtsiTs102941MessagesCa"
 * 	found in "asn1/pki_ts102941/EtsiTs102941MessagesCA.asn"
 * 	`asn1c -S ../../../../asn1c-fillabs2/skeletons -fcompound-names -no-gen-BER -no-gen-JER -no-gen-UPER -no-gen-APER -no-gen-random-fill -no-gen-example -pdu=EtsiTs103097Data -pdu=EnrolmentRequestMessage -pdu=EnrolmentResponseMessage -pdu=AuthorizationRequestMessage -pdu=AuthorizationRequestMessageWithPop -pdu=AuthorizationResponseMessage -pdu=CertificateRevocationListMessage -pdu=TlmCertificateTrustListMessage -pdu=RcaCertificateTrustListMessage -pdu=AuthorizationValidationRequestMessage -pdu=AuthorizationValidationResponseMessage -pdu=CaCertificateRequestMessage -pdu=EtsiTs103097Certificate`
 */


/* Including external dependencies */
#include "EtsiTs103097Data-SignedAndEncrypted-Unicast.h"
#ifndef	_EnrolmentResponseMessage_H_
#define	_EnrolmentResponseMessage_H_


#include <asn_application.h>

#ifdef __cplusplus
extern "C" {
#endif

/* EnrolmentResponseMessage */
typedef EtsiTs103097Data_SignedAndEncrypted_Unicast_115P0_t	 EnrolmentResponseMessage_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_EnrolmentResponseMessage;
asn_struct_free_f EnrolmentResponseMessage_free;
asn_struct_print_f EnrolmentResponseMessage_print;
asn_constr_check_f EnrolmentResponseMessage_constraint;
xer_type_decoder_f EnrolmentResponseMessage_decode_xer;
xer_type_encoder_f EnrolmentResponseMessage_encode_xer;
oer_type_decoder_f EnrolmentResponseMessage_decode_oer;
oer_type_encoder_f EnrolmentResponseMessage_encode_oer;

#ifdef __cplusplus
}
#endif

#endif	/* _EnrolmentResponseMessage_H_ */
#include <asn_internal.h>