/*
 * Generated by asn1c-0.9.29-DF (http://lionet.info/asn1c)
 * From ASN.1 module "EtsiTs102941MessagesCa"
 * 	found in "asn1/pki_ts102941/EtsiTs102941MessagesCA.asn"
 * 	`asn1c -S ../../../../asn1c-fillabs2/skeletons -fcompound-names -no-gen-BER -no-gen-JER -no-gen-UPER -no-gen-APER -no-gen-random-fill -no-gen-example -pdu=EtsiTs103097Data -pdu=EnrolmentRequestMessage -pdu=EnrolmentResponseMessage -pdu=AuthorizationRequestMessage -pdu=AuthorizationRequestMessageWithPop -pdu=AuthorizationResponseMessage -pdu=CertificateRevocationListMessage -pdu=TlmCertificateTrustListMessage -pdu=RcaCertificateTrustListMessage -pdu=AuthorizationValidationRequestMessage -pdu=AuthorizationValidationResponseMessage -pdu=CaCertificateRequestMessage -pdu=EtsiTs103097Certificate`
 */


/* Including external dependencies */
#include "EtsiTs103097Data-SignedAndEncrypted-Unicast.h"
#ifndef	_EnrolmentRequestMessage_H_
#define	_EnrolmentRequestMessage_H_


#include <asn_application.h>

#ifdef __cplusplus
extern "C" {
#endif

/* EnrolmentRequestMessage */
typedef EtsiTs103097Data_SignedAndEncrypted_Unicast_115P0_t	 EnrolmentRequestMessage_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_EnrolmentRequestMessage;
asn_struct_free_f EnrolmentRequestMessage_free;
asn_struct_print_f EnrolmentRequestMessage_print;
asn_constr_check_f EnrolmentRequestMessage_constraint;
xer_type_decoder_f EnrolmentRequestMessage_decode_xer;
xer_type_encoder_f EnrolmentRequestMessage_encode_xer;
oer_type_decoder_f EnrolmentRequestMessage_decode_oer;
oer_type_encoder_f EnrolmentRequestMessage_encode_oer;

#ifdef __cplusplus
}
#endif

#endif	/* _EnrolmentRequestMessage_H_ */
#include <asn_internal.h>
