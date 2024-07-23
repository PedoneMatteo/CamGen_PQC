/*
 * Generated by asn1c-0.9.29-DF (http://lionet.info/asn1c)
 * From ASN.1 module "ITS-Container"
 * 	found in "asn1/ITS-Container.asn"
 * 	`asn1c -S ../../asn1c-fillabs/skeletons -no-gen-example -no-gen-APER -no-gen-OER -no-gen-XER -pdu=CAM -pdu=DENM`
 */


/* Including external dependencies */
#include <NativeInteger.h>
#ifndef	_WrongWayDrivingSubCauseCode_H_
#define	_WrongWayDrivingSubCauseCode_H_


#include <asn_application.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum WrongWayDrivingSubCauseCode {
	WrongWayDrivingSubCauseCode_unavailable	= 0,
	WrongWayDrivingSubCauseCode_wrongLane	= 1,
	WrongWayDrivingSubCauseCode_wrongDirection	= 2
} e_WrongWayDrivingSubCauseCode;

/* WrongWayDrivingSubCauseCode */
typedef long	 WrongWayDrivingSubCauseCode_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_WrongWayDrivingSubCauseCode;
asn_struct_free_f WrongWayDrivingSubCauseCode_free;
asn_struct_print_f WrongWayDrivingSubCauseCode_print;
asn_constr_check_f WrongWayDrivingSubCauseCode_constraint;
ber_type_decoder_f WrongWayDrivingSubCauseCode_decode_ber;
der_type_encoder_f WrongWayDrivingSubCauseCode_encode_der;
jer_type_encoder_f WrongWayDrivingSubCauseCode_encode_jer;
per_type_decoder_f WrongWayDrivingSubCauseCode_decode_uper;
per_type_encoder_f WrongWayDrivingSubCauseCode_encode_uper;

#ifdef __cplusplus
}
#endif

#endif	/* _WrongWayDrivingSubCauseCode_H_ */
#include <asn_internal.h>