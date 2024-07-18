/*
 * Generated by asn1c-0.9.29-DF (http://lionet.info/asn1c)
 * From ASN.1 module "EtsiTs102941TrustLists"
 * 	found in "asn1/pki_ts102941/EtsiTs102941TrustLists.asn"
 * 	`asn1c -S ../../../../asn1c-fillabs2/skeletons -fcompound-names -no-gen-BER -no-gen-JER -no-gen-UPER -no-gen-APER -no-gen-random-fill -no-gen-example -pdu=EtsiTs103097Data -pdu=EnrolmentRequestMessage -pdu=EnrolmentResponseMessage -pdu=AuthorizationRequestMessage -pdu=AuthorizationRequestMessageWithPop -pdu=AuthorizationResponseMessage -pdu=CertificateRevocationListMessage -pdu=TlmCertificateTrustListMessage -pdu=RcaCertificateTrustListMessage -pdu=AuthorizationValidationRequestMessage -pdu=AuthorizationValidationResponseMessage -pdu=CaCertificateRequestMessage -pdu=EtsiTs103097Certificate`
 */


/* Including external dependencies */
#include "CtlFormat.h"
#ifndef	_ToBeSignedRcaCtl_H_
#define	_ToBeSignedRcaCtl_H_


#include <asn_application.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ToBeSignedRcaCtl */
typedef CtlFormat_t	 ToBeSignedRcaCtl_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_ToBeSignedRcaCtl;
asn_struct_free_f ToBeSignedRcaCtl_free;
asn_struct_print_f ToBeSignedRcaCtl_print;
asn_constr_check_f ToBeSignedRcaCtl_constraint;
xer_type_decoder_f ToBeSignedRcaCtl_decode_xer;
xer_type_encoder_f ToBeSignedRcaCtl_encode_xer;
oer_type_decoder_f ToBeSignedRcaCtl_decode_oer;
oer_type_encoder_f ToBeSignedRcaCtl_encode_oer;

#ifdef __cplusplus
}
#endif

#endif	/* _ToBeSignedRcaCtl_H_ */
#include <asn_internal.h>
