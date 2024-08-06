/*
 * Generated by asn1c-0.9.29-DF (http://lionet.info/asn1c)
 * From ASN.1 module "EtsiTs103097Module"
 * 	found in "asn1/sec_ts103097/EtsiTs103097Module.asn"
 * 	`asn1c -fcompound-names -no-gen-BER -no-gen-JER -no-gen-UPER -no-gen-APER -no-gen-random-fill -no-gen-example -pdu=EtsiTs103097Data -pdu=EnrolmentRequestMessage -pdu=EnrolmentResponseMessage -pdu=AuthorizationRequestMessage -pdu=AuthorizationRequestMessageWithPop -pdu=AuthorizationResponseMessage -pdu=CertificateRevocationListMessage -pdu=TlmCertificateTrustListMessage -pdu=RcaCertificateTrustListMessage -pdu=AuthorizationValidationRequestMessage -pdu=AuthorizationValidationResponseMessage -pdu=CaCertificateRequestMessage -pdu=EtsiTs103097Certificate`
 */


/* Including external dependencies */
#include "Ieee1609Dot2Data.h"
#ifndef	_EtsiTs103097Data_H_
#define	_EtsiTs103097Data_H_


#include <asn_application.h>

#ifdef __cplusplus
extern "C" {
#endif

/* EtsiTs103097Data */
typedef Ieee1609Dot2Data_t	 EtsiTs103097Data_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_EtsiTs103097Data;
asn_struct_free_f EtsiTs103097Data_free;
asn_struct_print_f EtsiTs103097Data_print;
asn_constr_check_f EtsiTs103097Data_constraint;
xer_type_decoder_f EtsiTs103097Data_decode_xer;
xer_type_encoder_f EtsiTs103097Data_encode_xer;
oer_type_decoder_f EtsiTs103097Data_decode_oer;
oer_type_encoder_f EtsiTs103097Data_encode_oer;

#ifdef __cplusplus
}
#endif

#endif	/* _EtsiTs103097Data_H_ */
#include <asn_internal.h>
