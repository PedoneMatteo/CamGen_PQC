/*
 * Generated by asn1c-0.9.29-DF (http://lionet.info/asn1c)
 * From ASN.1 module "ITS-Container"
 * 	found in "asn1/ITS-Container.asn"
 * 	`asn1c -S ../../asn1c-fillabs/skeletons -no-gen-example -no-gen-APER -no-gen-OER -no-gen-XER -pdu=CAM -pdu=DENM`
 */


/* Including external dependencies */
#include "VerticalAccelerationValue.h"
#include "AccelerationConfidence.h"
#include <constr_SEQUENCE.h>
#ifndef	_VerticalAcceleration_H_
#define	_VerticalAcceleration_H_


#include <asn_application.h>

#ifdef __cplusplus
extern "C" {
#endif

/* VerticalAcceleration */
typedef struct VerticalAcceleration {
	VerticalAccelerationValue_t	 verticalAccelerationValue;
	AccelerationConfidence_t	 verticalAccelerationConfidence;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} VerticalAcceleration_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_VerticalAcceleration;
extern asn_SEQUENCE_specifics_t asn_SPC_VerticalAcceleration_specs_1;
extern asn_TYPE_member_t asn_MBR_VerticalAcceleration_1[2];

#ifdef __cplusplus
}
#endif

#endif	/* _VerticalAcceleration_H_ */
#include <asn_internal.h>
