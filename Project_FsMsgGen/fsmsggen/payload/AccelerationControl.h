/*
 * Generated by asn1c-0.9.29-DF (http://lionet.info/asn1c)
 * From ASN.1 module "ITS-Container"
 * 	found in "asn1/ITS-Container.asn"
 * 	`asn1c -S ../../asn1c-fillabs/skeletons -no-gen-example -no-gen-APER -no-gen-OER -no-gen-XER -pdu=CAM -pdu=DENM`
 */


/* Including external dependencies */
#include <BIT_STRING.h>
#ifndef	_AccelerationControl_H_
#define	_AccelerationControl_H_


#include <asn_application.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum AccelerationControl {
	AccelerationControl_brakePedalEngaged	= 0,
	AccelerationControl_gasPedalEngaged	= 1,
	AccelerationControl_emergencyBrakeEngaged	= 2,
	AccelerationControl_collisionWarningEngaged	= 3,
	AccelerationControl_accEngaged	= 4,
	AccelerationControl_cruiseControlEngaged	= 5,
	AccelerationControl_speedLimiterEngaged	= 6
} e_AccelerationControl;

/* AccelerationControl */
typedef BIT_STRING_t	 AccelerationControl_t;

/* Implementation */
extern asn_per_constraints_t asn_PER_type_AccelerationControl_constr_1;
extern asn_TYPE_descriptor_t asn_DEF_AccelerationControl;
asn_struct_free_f AccelerationControl_free;
asn_struct_print_f AccelerationControl_print;
asn_constr_check_f AccelerationControl_constraint;
ber_type_decoder_f AccelerationControl_decode_ber;
der_type_encoder_f AccelerationControl_encode_der;
jer_type_encoder_f AccelerationControl_encode_jer;
per_type_decoder_f AccelerationControl_decode_uper;
per_type_encoder_f AccelerationControl_encode_uper;

#ifdef __cplusplus
}
#endif

#endif	/* _AccelerationControl_H_ */
#include <asn_internal.h>