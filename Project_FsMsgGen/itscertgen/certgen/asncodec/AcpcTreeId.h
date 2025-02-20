/*
 * Generated by asn1c-0.9.29-DF (http://lionet.info/asn1c)
 * From ASN.1 module "Ieee1609Dot2Dot1Acpc"
 * 	found in "asn1/ieee1609.2.1/Ieee1609Dot2Dot1Acpc.asn"
 * 	`asn1c -fcompound-names -no-gen-BER -no-gen-JER -no-gen-UPER -no-gen-APER -no-gen-random-fill -no-gen-example -pdu=EtsiTs103097Data -pdu=EnrolmentRequestMessage -pdu=EnrolmentResponseMessage -pdu=AuthorizationRequestMessage -pdu=AuthorizationRequestMessageWithPop -pdu=AuthorizationResponseMessage -pdu=CertificateRevocationListMessage -pdu=TlmCertificateTrustListMessage -pdu=RcaCertificateTrustListMessage -pdu=AuthorizationValidationRequestMessage -pdu=AuthorizationValidationResponseMessage -pdu=CaCertificateRequestMessage -pdu=EtsiTs103097Certificate`
 */


/* Including external dependencies */
#include <OCTET_STRING.h>
#ifndef	_AcpcTreeId_H_
#define	_AcpcTreeId_H_


#include <asn_application.h>

#ifdef __cplusplus
extern "C" {
#endif

/* AcpcTreeId */
typedef OCTET_STRING_t	 AcpcTreeId_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_AcpcTreeId;
asn_struct_free_f AcpcTreeId_free;
asn_struct_print_f AcpcTreeId_print;
asn_constr_check_f AcpcTreeId_constraint;
xer_type_decoder_f AcpcTreeId_decode_xer;
xer_type_encoder_f AcpcTreeId_encode_xer;
oer_type_decoder_f AcpcTreeId_decode_oer;
oer_type_encoder_f AcpcTreeId_encode_oer;

#ifdef __cplusplus
}
#endif

#endif	/* _AcpcTreeId_H_ */
#include <asn_internal.h>
