/*
 * Generated by asn1c-0.9.29-DF (http://lionet.info/asn1c)
 * From ASN.1 module "ITS-Container"
 * 	found in "asn1/ITS-Container.asn"
 * 	`asn1c -S ../../asn1c-fillabs/skeletons -no-gen-example -no-gen-APER -no-gen-OER -no-gen-XER -pdu=CAM -pdu=DENM`
 */


/* Including external dependencies */
#include <NativeInteger.h>
#ifndef	_ValidityDuration_H_
#define	_ValidityDuration_H_


#include <asn_application.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum ValidityDuration {
	ValidityDuration_timeOfDetection	= 0,
	ValidityDuration_oneSecondAfterDetection	= 1
} e_ValidityDuration;

/* ValidityDuration */
typedef long	 ValidityDuration_t;

/* Implementation */
extern asn_per_constraints_t asn_PER_type_ValidityDuration_constr_1;
extern asn_TYPE_descriptor_t asn_DEF_ValidityDuration;
asn_struct_free_f ValidityDuration_free;
asn_struct_print_f ValidityDuration_print;
asn_constr_check_f ValidityDuration_constraint;
ber_type_decoder_f ValidityDuration_decode_ber;
der_type_encoder_f ValidityDuration_encode_der;
jer_type_encoder_f ValidityDuration_encode_jer;
per_type_decoder_f ValidityDuration_decode_uper;
per_type_encoder_f ValidityDuration_encode_uper;

#ifdef __cplusplus
}
#endif

#endif	/* _ValidityDuration_H_ */
#include <asn_internal.h>
