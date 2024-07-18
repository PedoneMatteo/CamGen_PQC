/*
 * Generated by asn1c-0.9.29-DF (http://lionet.info/asn1c)
 * From ASN.1 module "ITS-Container"
 * 	found in "asn1/ITS-Container.asn"
 * 	`asn1c -S ../../asn1c-fillabs/skeletons -no-gen-example -no-gen-APER -no-gen-OER -no-gen-XER -pdu=CAM -pdu=DENM`
 */


/* Including external dependencies */
#include "SemiAxisLength.h"
#include "HeadingValue.h"
#include <constr_SEQUENCE.h>
#ifndef	_PosConfidenceEllipse_H_
#define	_PosConfidenceEllipse_H_


#include <asn_application.h>

#ifdef __cplusplus
extern "C" {
#endif

/* PosConfidenceEllipse */
typedef struct PosConfidenceEllipse {
	SemiAxisLength_t	 semiMajorConfidence;
	SemiAxisLength_t	 semiMinorConfidence;
	HeadingValue_t	 semiMajorOrientation;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} PosConfidenceEllipse_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_PosConfidenceEllipse;
extern asn_SEQUENCE_specifics_t asn_SPC_PosConfidenceEllipse_specs_1;
extern asn_TYPE_member_t asn_MBR_PosConfidenceEllipse_1[3];

#ifdef __cplusplus
}
#endif

#endif	/* _PosConfidenceEllipse_H_ */
#include <asn_internal.h>
