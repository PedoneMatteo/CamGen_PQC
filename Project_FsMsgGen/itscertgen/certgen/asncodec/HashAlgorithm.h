/*
 * Generated by asn1c-0.9.29-DF (http://lionet.info/asn1c)
 * From ASN.1 module "Ieee1609Dot2BaseTypes"
 * 	found in "asn1/ieee1609.2/IEEE1609dot2BaseTypes.asn"
 * 	`asn1c -S ../../../../asn1c-fillabs2/skeletons -fcompound-names -no-gen-BER -no-gen-JER -no-gen-UPER -no-gen-APER -no-gen-random-fill -no-gen-example -pdu=EtsiTs103097Data -pdu=EnrolmentRequestMessage -pdu=EnrolmentResponseMessage -pdu=AuthorizationRequestMessage -pdu=AuthorizationRequestMessageWithPop -pdu=AuthorizationResponseMessage -pdu=CertificateRevocationListMessage -pdu=TlmCertificateTrustListMessage -pdu=RcaCertificateTrustListMessage -pdu=AuthorizationValidationRequestMessage -pdu=AuthorizationValidationResponseMessage -pdu=CaCertificateRequestMessage -pdu=EtsiTs103097Certificate`
 */


/* Including external dependencies */
#include <NativeEnumerated.h>
#ifndef	_HashAlgorithm_H_
#define	_HashAlgorithm_H_


#include <asn_application.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum HashAlgorithm {
	HashAlgorithm_sha256	= 0,
	/*
	 * Enumeration is extensible
	 */
	HashAlgorithm_sha384	= 1,
	HashAlgorithm_sm3	= 2
} e_HashAlgorithm;

/* HashAlgorithm */
typedef long	 HashAlgorithm_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_HashAlgorithm;
extern const asn_INTEGER_specifics_t asn_SPC_HashAlgorithm_specs_1;
asn_struct_free_f HashAlgorithm_free;
asn_struct_print_f HashAlgorithm_print;
asn_constr_check_f HashAlgorithm_constraint;
xer_type_decoder_f HashAlgorithm_decode_xer;
xer_type_encoder_f HashAlgorithm_encode_xer;
oer_type_decoder_f HashAlgorithm_decode_oer;
oer_type_encoder_f HashAlgorithm_encode_oer;

#ifdef __cplusplus
}
#endif

#endif	/* _HashAlgorithm_H_ */
#include <asn_internal.h>