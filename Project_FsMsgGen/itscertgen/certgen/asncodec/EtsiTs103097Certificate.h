/*
 * Generated by asn1c-0.9.29-DF (http://lionet.info/asn1c)
 * From ASN.1 module "EtsiTs103097Module"
 * 	found in "asn1/sec_ts103097/EtsiTs103097Module.asn"
 * 	`asn1c -fcompound-names -no-gen-BER -no-gen-JER -no-gen-UPER -no-gen-APER -no-gen-random-fill -no-gen-example -pdu=EtsiTs103097Data -pdu=EnrolmentRequestMessage -pdu=EnrolmentResponseMessage -pdu=AuthorizationRequestMessage -pdu=AuthorizationRequestMessageWithPop -pdu=AuthorizationResponseMessage -pdu=CertificateRevocationListMessage -pdu=TlmCertificateTrustListMessage -pdu=RcaCertificateTrustListMessage -pdu=AuthorizationValidationRequestMessage -pdu=AuthorizationValidationResponseMessage -pdu=CaCertificateRequestMessage -pdu=EtsiTs103097Certificate`
 */


/* Including external dependencies */
#include "Certificate.h"
#ifndef	_EtsiTs103097Certificate_H_
#define	_EtsiTs103097Certificate_H_


#include <asn_application.h>

#ifdef __cplusplus
extern "C" {
#endif

/* EtsiTs103097Certificate */
typedef Certificate_t	 EtsiTs103097Certificate_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_EtsiTs103097Certificate;
asn_struct_free_f EtsiTs103097Certificate_free;
asn_struct_print_f EtsiTs103097Certificate_print;
asn_constr_check_f EtsiTs103097Certificate_constraint;
xer_type_decoder_f EtsiTs103097Certificate_decode_xer;
xer_type_encoder_f EtsiTs103097Certificate_encode_xer;
oer_type_decoder_f EtsiTs103097Certificate_decode_oer;
oer_type_encoder_f EtsiTs103097Certificate_encode_oer;

#ifdef __cplusplus
}
#endif

#endif	/* _EtsiTs103097Certificate_H_ */
#include <asn_internal.h>
