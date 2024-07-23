/*
 * Generated by asn1c-0.9.29-DF (http://lionet.info/asn1c)
 * From ASN.1 module "ITS-Container"
 * 	found in "asn1/ITS-Container.asn"
 * 	`asn1c -S ../../asn1c-fillabs/skeletons -no-gen-example -no-gen-APER -no-gen-OER -no-gen-XER -pdu=CAM -pdu=DENM`
 */


/* Including external dependencies */
#include <NativeInteger.h>
#ifndef	_DangerousEndOfQueueSubCauseCode_H_
#define	_DangerousEndOfQueueSubCauseCode_H_


#include <asn_application.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum DangerousEndOfQueueSubCauseCode {
	DangerousEndOfQueueSubCauseCode_unavailable	= 0,
	DangerousEndOfQueueSubCauseCode_suddenEndOfQueue	= 1,
	DangerousEndOfQueueSubCauseCode_queueOverHill	= 2,
	DangerousEndOfQueueSubCauseCode_queueAroundBend	= 3,
	DangerousEndOfQueueSubCauseCode_queueInTunnel	= 4
} e_DangerousEndOfQueueSubCauseCode;

/* DangerousEndOfQueueSubCauseCode */
typedef long	 DangerousEndOfQueueSubCauseCode_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_DangerousEndOfQueueSubCauseCode;
asn_struct_free_f DangerousEndOfQueueSubCauseCode_free;
asn_struct_print_f DangerousEndOfQueueSubCauseCode_print;
asn_constr_check_f DangerousEndOfQueueSubCauseCode_constraint;
ber_type_decoder_f DangerousEndOfQueueSubCauseCode_decode_ber;
der_type_encoder_f DangerousEndOfQueueSubCauseCode_encode_der;
jer_type_encoder_f DangerousEndOfQueueSubCauseCode_encode_jer;
per_type_decoder_f DangerousEndOfQueueSubCauseCode_decode_uper;
per_type_encoder_f DangerousEndOfQueueSubCauseCode_encode_uper;

#ifdef __cplusplus
}
#endif

#endif	/* _DangerousEndOfQueueSubCauseCode_H_ */
#include <asn_internal.h>