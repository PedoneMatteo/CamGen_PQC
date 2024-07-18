/*
 * Generated by asn1c-0.9.29-DF (http://lionet.info/asn1c)
 * From ASN.1 module "ASN1C-DATE"
 * 	found in "../../asn1c-fillabs/skeletons/standard-modules/date.asn1"
 * 	`asn1c -S ../../asn1c-fillabs/skeletons -no-gen-example -no-gen-APER -no-gen-OER -no-gen-XER -pdu=CAM -pdu=DENM`
 */


/* Including external dependencies */
#include "DATE-ENCODING.h"
#ifndef	_DATE_H_
#define	_DATE_H_


#include <asn_application.h>

#ifdef __cplusplus
extern "C" {
#endif

/* DATE */
typedef DATE_ENCODING_t	 DATE_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_DATE;
asn_struct_free_f DATE_free;
asn_struct_print_f DATE_print;
asn_constr_check_f DATE_constraint;
ber_type_decoder_f DATE_decode_ber;
der_type_encoder_f DATE_encode_der;
jer_type_encoder_f DATE_encode_jer;
per_type_decoder_f DATE_decode_uper;
per_type_encoder_f DATE_encode_uper;

#ifdef __cplusplus
}
#endif

#endif	/* _DATE_H_ */
#include <asn_internal.h>
