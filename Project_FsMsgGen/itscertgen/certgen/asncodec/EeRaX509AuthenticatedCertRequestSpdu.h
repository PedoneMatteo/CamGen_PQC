/*
 * Generated by asn1c-0.9.29-DF (http://lionet.info/asn1c)
 * From ASN.1 module "Ieee1609Dot2Dot1Protocol"
 * 	found in "asn1/ieee1609.2.1/Ieee1609Dot2Dot1Protocol.asn"
 * 	`asn1c -S ../../../../asn1c-fillabs2/skeletons -fcompound-names -no-gen-BER -no-gen-JER -no-gen-UPER -no-gen-APER -no-gen-random-fill -no-gen-example -pdu=EtsiTs103097Data -pdu=EnrolmentRequestMessage -pdu=EnrolmentResponseMessage -pdu=AuthorizationRequestMessage -pdu=AuthorizationRequestMessageWithPop -pdu=AuthorizationResponseMessage -pdu=CertificateRevocationListMessage -pdu=TlmCertificateTrustListMessage -pdu=RcaCertificateTrustListMessage -pdu=AuthorizationValidationRequestMessage -pdu=AuthorizationValidationResponseMessage -pdu=CaCertificateRequestMessage -pdu=EtsiTs103097Certificate`
 */


/* Including external dependencies */
#include "Ieee1609Dot2Data-Encrypted.h"
#ifndef	_EeRaX509AuthenticatedCertRequestSpdu_H_
#define	_EeRaX509AuthenticatedCertRequestSpdu_H_


#include <asn_application.h>

#ifdef __cplusplus
extern "C" {
#endif

/* EeRaX509AuthenticatedCertRequestSpdu */
typedef Ieee1609Dot2Data_Encrypted_276P2_t	 EeRaX509AuthenticatedCertRequestSpdu_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_EeRaX509AuthenticatedCertRequestSpdu;
asn_struct_free_f EeRaX509AuthenticatedCertRequestSpdu_free;
asn_struct_print_f EeRaX509AuthenticatedCertRequestSpdu_print;
asn_constr_check_f EeRaX509AuthenticatedCertRequestSpdu_constraint;
xer_type_decoder_f EeRaX509AuthenticatedCertRequestSpdu_decode_xer;
xer_type_encoder_f EeRaX509AuthenticatedCertRequestSpdu_encode_xer;
oer_type_decoder_f EeRaX509AuthenticatedCertRequestSpdu_decode_oer;
oer_type_encoder_f EeRaX509AuthenticatedCertRequestSpdu_encode_oer;

#ifdef __cplusplus
}
#endif

#endif	/* _EeRaX509AuthenticatedCertRequestSpdu_H_ */
#include <asn_internal.h>