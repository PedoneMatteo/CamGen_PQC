/*
 * Generated by asn1c-0.9.29-DF (http://lionet.info/asn1c)
 * From ASN.1 module "Ieee1609Dot2BaseTypes"
 * 	found in "asn1/ieee1609.2/IEEE1609dot2BaseTypes.asn"
 * 	`asn1c -S ../../../../asn1c-fillabs2/skeletons -fcompound-names -no-gen-BER -no-gen-JER -no-gen-UPER -no-gen-APER -no-gen-random-fill -no-gen-example -pdu=EtsiTs103097Data -pdu=EnrolmentRequestMessage -pdu=EnrolmentResponseMessage -pdu=AuthorizationRequestMessage -pdu=AuthorizationRequestMessageWithPop -pdu=AuthorizationResponseMessage -pdu=CertificateRevocationListMessage -pdu=TlmCertificateTrustListMessage -pdu=RcaCertificateTrustListMessage -pdu=AuthorizationValidationRequestMessage -pdu=AuthorizationValidationResponseMessage -pdu=CaCertificateRequestMessage -pdu=EtsiTs103097Certificate`
 */


/* Including external dependencies */
#include "Uint16.h"
#ifndef	_IValue_H_
#define	_IValue_H_


#include <asn_application.h>

#ifdef __cplusplus
extern "C" {
#endif

/* IValue */
typedef Uint16_t	 IValue_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_IValue;
asn_struct_free_f IValue_free;
asn_struct_print_f IValue_print;
asn_constr_check_f IValue_constraint;
xer_type_decoder_f IValue_decode_xer;
xer_type_encoder_f IValue_encode_xer;
oer_type_decoder_f IValue_decode_oer;
oer_type_encoder_f IValue_encode_oer;

#ifdef __cplusplus
}
#endif

#endif	/* _IValue_H_ */
#include <asn_internal.h>
