/*
 * Generated by asn1c-0.9.29-DF (http://lionet.info/asn1c)
 * From ASN.1 module "Ieee1609Dot2BaseTypes"
 * 	found in "asn1/ieee1609.2/Ieee1609Dot2BaseTypes.asn"
 * 	`asn1c -fcompound-names -no-gen-BER -no-gen-JER -no-gen-UPER -no-gen-APER -no-gen-random-fill -no-gen-example -pdu=EtsiTs103097Data -pdu=EnrolmentRequestMessage -pdu=EnrolmentResponseMessage -pdu=AuthorizationRequestMessage -pdu=AuthorizationRequestMessageWithPop -pdu=AuthorizationResponseMessage -pdu=CertificateRevocationListMessage -pdu=TlmCertificateTrustListMessage -pdu=RcaCertificateTrustListMessage -pdu=AuthorizationValidationRequestMessage -pdu=AuthorizationValidationResponseMessage -pdu=CaCertificateRequestMessage -pdu=EtsiTs103097Certificate`
 */


/* Including external dependencies */
#include <NativeInteger.h>
#ifndef	_OneEightyDegreeInt_H_
#define	_OneEightyDegreeInt_H_


#include <asn_application.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum OneEightyDegreeInt {
	OneEightyDegreeInt_min	= -1799999999,
	OneEightyDegreeInt_max	= 1800000000,
	OneEightyDegreeInt_unknown	= 1800000001
} e_OneEightyDegreeInt;

/* OneEightyDegreeInt */
typedef long	 OneEightyDegreeInt_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_OneEightyDegreeInt;
asn_struct_free_f OneEightyDegreeInt_free;
asn_struct_print_f OneEightyDegreeInt_print;
asn_constr_check_f OneEightyDegreeInt_constraint;
xer_type_decoder_f OneEightyDegreeInt_decode_xer;
xer_type_encoder_f OneEightyDegreeInt_encode_xer;
oer_type_decoder_f OneEightyDegreeInt_decode_oer;
oer_type_encoder_f OneEightyDegreeInt_encode_oer;

#ifdef __cplusplus
}
#endif

#endif	/* _OneEightyDegreeInt_H_ */
#include <asn_internal.h>
