/*
 * Generated by asn1c-0.9.29-DF (http://lionet.info/asn1c)
 * From ASN.1 module "ASN1C-DATE"
 * 	found in "../../asn1c-fillabs/skeletons/standard-modules/date.asn1"
 * 	`asn1c -S ../../asn1c-fillabs/skeletons -no-gen-example -no-gen-APER -no-gen-OER -no-gen-XER -pdu=CAM -pdu=DENM`
 */


/* Including external dependencies */
#include <NativeInteger.h>
#include <constr_CHOICE.h>
#ifndef	_YEAR_ENCODING_H_
#define	_YEAR_ENCODING_H_


#include <asn_application.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum YEAR_ENCODING_PR {
	YEAR_ENCODING_PR_NOTHING,	/* No components present */
	YEAR_ENCODING_PR_immediate,
	YEAR_ENCODING_PR_near_future,
	YEAR_ENCODING_PR_near_past,
	YEAR_ENCODING_PR_remainder
} YEAR_ENCODING_PR;

/* YEAR-ENCODING */
typedef struct YEAR_ENCODING {
	YEAR_ENCODING_PR present;
	union YEAR_ENCODING_u {
		long	 immediate;
		long	 near_future;
		long	 near_past;
		long	 remainder;
	} choice;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} YEAR_ENCODING_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_YEAR_ENCODING;
extern asn_CHOICE_specifics_t asn_SPC_YEAR_ENCODING_specs_1;
extern asn_TYPE_member_t asn_MBR_YEAR_ENCODING_1[4];
extern asn_per_constraints_t asn_PER_type_YEAR_ENCODING_constr_1;

#ifdef __cplusplus
}
#endif

#endif	/* _YEAR_ENCODING_H_ */
#include <asn_internal.h>
