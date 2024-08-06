/*
 * Generated by asn1c-0.9.29-DF (http://lionet.info/asn1c)
 * From ASN.1 module "EtsiTs102941TrustLists"
 * 	found in "asn1/pki_ts102941/EtsiTs102941TrustLists.asn"
 * 	`asn1c -fcompound-names -no-gen-BER -no-gen-JER -no-gen-UPER -no-gen-APER -no-gen-random-fill -no-gen-example -pdu=EtsiTs103097Data -pdu=EnrolmentRequestMessage -pdu=EnrolmentResponseMessage -pdu=AuthorizationRequestMessage -pdu=AuthorizationRequestMessageWithPop -pdu=AuthorizationResponseMessage -pdu=CertificateRevocationListMessage -pdu=TlmCertificateTrustListMessage -pdu=RcaCertificateTrustListMessage -pdu=AuthorizationValidationRequestMessage -pdu=AuthorizationValidationResponseMessage -pdu=CaCertificateRequestMessage -pdu=EtsiTs103097Certificate`
 */


/* Including external dependencies */
#include "HashedId8.h"
#ifndef	_CrlEntry_H_
#define	_CrlEntry_H_


#include <asn_application.h>

#ifdef __cplusplus
extern "C" {
#endif

/* CrlEntry */
typedef HashedId8_t	 CrlEntry_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_CrlEntry;
asn_struct_free_f CrlEntry_free;
asn_struct_print_f CrlEntry_print;
asn_constr_check_f CrlEntry_constraint;
xer_type_decoder_f CrlEntry_decode_xer;
xer_type_encoder_f CrlEntry_encode_xer;
oer_type_decoder_f CrlEntry_decode_oer;
oer_type_encoder_f CrlEntry_encode_oer;

#ifdef __cplusplus
}
#endif

#endif	/* _CrlEntry_H_ */
#include <asn_internal.h>
