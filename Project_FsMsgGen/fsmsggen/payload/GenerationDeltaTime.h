/*
 * Generated by asn1c-0.9.29-DF (http://lionet.info/asn1c)
 * From ASN.1 module "CAM-PDU-Descriptions"
 * 	found in "asn1/CAM-PDU-Descriptions.asn"
 * 	`asn1c -S ../../asn1c-fillabs/skeletons -no-gen-example -no-gen-APER -no-gen-OER -no-gen-XER -pdu=CAM -pdu=DENM`
 */


/* Including external dependencies */
#include <NativeInteger.h>
#ifndef	_GenerationDeltaTime_H_
#define	_GenerationDeltaTime_H_


#include <asn_application.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum GenerationDeltaTime {
	GenerationDeltaTime_oneMilliSec	= 1
} e_GenerationDeltaTime;

/* GenerationDeltaTime */
typedef long	 GenerationDeltaTime_t;

/* Implementation */
extern asn_per_constraints_t asn_PER_type_GenerationDeltaTime_constr_1;
extern asn_TYPE_descriptor_t asn_DEF_GenerationDeltaTime;
asn_struct_free_f GenerationDeltaTime_free;
asn_struct_print_f GenerationDeltaTime_print;
asn_constr_check_f GenerationDeltaTime_constraint;
ber_type_decoder_f GenerationDeltaTime_decode_ber;
der_type_encoder_f GenerationDeltaTime_encode_der;
jer_type_encoder_f GenerationDeltaTime_encode_jer;
per_type_decoder_f GenerationDeltaTime_decode_uper;
per_type_encoder_f GenerationDeltaTime_encode_uper;

#ifdef __cplusplus
}
#endif

#endif	/* _GenerationDeltaTime_H_ */
#include <asn_internal.h>
