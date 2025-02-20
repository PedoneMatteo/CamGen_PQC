/*
 * Generated by asn1c-0.9.29-DF (http://lionet.info/asn1c)
 * From ASN.1 module "Ieee1609Dot2"
 * 	found in "asn1/ieee1609.2/Ieee1609Dot2.asn"
 * 	`asn1c -fcompound-names -no-gen-BER -no-gen-JER -no-gen-UPER -no-gen-APER -no-gen-random-fill -no-gen-example -pdu=EtsiTs103097Data -pdu=EnrolmentRequestMessage -pdu=EnrolmentResponseMessage -pdu=AuthorizationRequestMessage -pdu=AuthorizationRequestMessageWithPop -pdu=AuthorizationResponseMessage -pdu=CertificateRevocationListMessage -pdu=TlmCertificateTrustListMessage -pdu=RcaCertificateTrustListMessage -pdu=AuthorizationValidationRequestMessage -pdu=AuthorizationValidationResponseMessage -pdu=CaCertificateRequestMessage -pdu=EtsiTs103097Certificate`
 */


/* Including external dependencies */
#include <OBJECT_IDENTIFIER.h>
#ifndef	_OperatingOrganizationId_H_
#define	_OperatingOrganizationId_H_


#include <asn_application.h>

#ifdef __cplusplus
extern "C" {
#endif

/* OperatingOrganizationId */
typedef OBJECT_IDENTIFIER_t	 OperatingOrganizationId_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_OperatingOrganizationId;
asn_struct_free_f OperatingOrganizationId_free;
asn_struct_print_f OperatingOrganizationId_print;
asn_constr_check_f OperatingOrganizationId_constraint;
xer_type_decoder_f OperatingOrganizationId_decode_xer;
xer_type_encoder_f OperatingOrganizationId_encode_xer;
oer_type_decoder_f OperatingOrganizationId_decode_oer;
oer_type_encoder_f OperatingOrganizationId_encode_oer;

#ifdef __cplusplus
}
#endif

#endif	/* _OperatingOrganizationId_H_ */
#include <asn_internal.h>
