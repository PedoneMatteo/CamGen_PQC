/*
 * Generated by asn1c-0.9.29-DF (http://lionet.info/asn1c)
 * From ASN.1 module "Ieee1609Dot2Dot1Protocol"
 * 	found in "asn1/ieee1609.2.1/Ieee1609Dot2Dot1Protocol.asn"
 * 	`asn1c -fcompound-names -no-gen-BER -no-gen-JER -no-gen-UPER -no-gen-APER -no-gen-random-fill -no-gen-example -pdu=EtsiTs103097Data -pdu=EnrolmentRequestMessage -pdu=EnrolmentResponseMessage -pdu=AuthorizationRequestMessage -pdu=AuthorizationRequestMessageWithPop -pdu=AuthorizationResponseMessage -pdu=CertificateRevocationListMessage -pdu=TlmCertificateTrustListMessage -pdu=RcaCertificateTrustListMessage -pdu=AuthorizationValidationRequestMessage -pdu=AuthorizationValidationResponseMessage -pdu=CaCertificateRequestMessage -pdu=EtsiTs103097Certificate`
 */


/* Including external dependencies */
#include "Ieee1609Dot2Data-EncryptedSigned.h"
#ifndef	_AcaEeCertResponsePrivateSpdu_H_
#define	_AcaEeCertResponsePrivateSpdu_H_


#include <asn_application.h>

#ifdef __cplusplus
extern "C" {
#endif

/* AcaEeCertResponsePrivateSpdu */
typedef Ieee1609Dot2Data_EncryptedSigned_379P0_t	 AcaEeCertResponsePrivateSpdu_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_AcaEeCertResponsePrivateSpdu;
asn_struct_free_f AcaEeCertResponsePrivateSpdu_free;
asn_struct_print_f AcaEeCertResponsePrivateSpdu_print;
asn_constr_check_f AcaEeCertResponsePrivateSpdu_constraint;
xer_type_decoder_f AcaEeCertResponsePrivateSpdu_decode_xer;
xer_type_encoder_f AcaEeCertResponsePrivateSpdu_encode_xer;
oer_type_decoder_f AcaEeCertResponsePrivateSpdu_decode_oer;
oer_type_encoder_f AcaEeCertResponsePrivateSpdu_encode_oer;

#ifdef __cplusplus
}
#endif

#endif	/* _AcaEeCertResponsePrivateSpdu_H_ */
#include <asn_internal.h>
