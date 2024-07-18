/*
 * Generated by asn1c-0.9.29-DF (http://lionet.info/asn1c)
 * From ASN.1 module "EtsiTs102941TypesAuthorizationValidation"
 * 	found in "asn1/pki_ts102941/EtsiTs102941TypesAuthorizationValidation.asn"
 * 	`asn1c -S ../../../../asn1c-fillabs2/skeletons -fcompound-names -no-gen-BER -no-gen-JER -no-gen-UPER -no-gen-APER -no-gen-random-fill -no-gen-example -pdu=EtsiTs103097Data -pdu=EnrolmentRequestMessage -pdu=EnrolmentResponseMessage -pdu=AuthorizationRequestMessage -pdu=AuthorizationRequestMessageWithPop -pdu=AuthorizationResponseMessage -pdu=CertificateRevocationListMessage -pdu=TlmCertificateTrustListMessage -pdu=RcaCertificateTrustListMessage -pdu=AuthorizationValidationRequestMessage -pdu=AuthorizationValidationResponseMessage -pdu=CaCertificateRequestMessage -pdu=EtsiTs103097Certificate`
 */


/* Including external dependencies */
#include <NativeEnumerated.h>
#ifndef	_AuthorizationValidationResponseCode_H_
#define	_AuthorizationValidationResponseCode_H_


#include <asn_application.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum AuthorizationValidationResponseCode {
	AuthorizationValidationResponseCode_ok	= 0,
	AuthorizationValidationResponseCode_cantparse	= 1,
	AuthorizationValidationResponseCode_badcontenttype	= 2,
	AuthorizationValidationResponseCode_imnottherecipient	= 3,
	AuthorizationValidationResponseCode_unknownencryptionalgorithm	= 4,
	AuthorizationValidationResponseCode_decryptionfailed	= 5,
	AuthorizationValidationResponseCode_invalidaa	= 6,
	AuthorizationValidationResponseCode_invalidaasignature	= 7,
	AuthorizationValidationResponseCode_wrongea	= 8,
	AuthorizationValidationResponseCode_unknownits	= 9,
	AuthorizationValidationResponseCode_invalidsignature	= 10,
	AuthorizationValidationResponseCode_invalidencryptionkey	= 11,
	AuthorizationValidationResponseCode_deniedpermissions	= 12,
	AuthorizationValidationResponseCode_deniedtoomanycerts	= 13,
	AuthorizationValidationResponseCode_deniedrequest	= 14
	/*
	 * Enumeration is extensible
	 */
} e_AuthorizationValidationResponseCode;

/* AuthorizationValidationResponseCode */
typedef long	 AuthorizationValidationResponseCode_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_AuthorizationValidationResponseCode;
extern const asn_INTEGER_specifics_t asn_SPC_AuthorizationValidationResponseCode_specs_1;
asn_struct_free_f AuthorizationValidationResponseCode_free;
asn_struct_print_f AuthorizationValidationResponseCode_print;
asn_constr_check_f AuthorizationValidationResponseCode_constraint;
xer_type_decoder_f AuthorizationValidationResponseCode_decode_xer;
xer_type_encoder_f AuthorizationValidationResponseCode_encode_xer;
oer_type_decoder_f AuthorizationValidationResponseCode_decode_oer;
oer_type_encoder_f AuthorizationValidationResponseCode_encode_oer;

#ifdef __cplusplus
}
#endif

#endif	/* _AuthorizationValidationResponseCode_H_ */
#include <asn_internal.h>
