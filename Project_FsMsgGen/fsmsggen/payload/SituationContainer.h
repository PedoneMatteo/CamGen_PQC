/*
 * Generated by asn1c-0.9.29-DF (http://lionet.info/asn1c)
 * From ASN.1 module "DENM-PDU-Descriptions"
 * 	found in "asn1/DENM-PDU-Descriptions.asn"
 * 	`asn1c -S ../../asn1c-fillabs/skeletons -no-gen-example -no-gen-APER -no-gen-OER -no-gen-XER -pdu=CAM -pdu=DENM`
 */


/* Including external dependencies */
#include "InformationQuality.h"
#include "CauseCode.h"
#include <constr_SEQUENCE.h>
#ifndef	_SituationContainer_H_
#define	_SituationContainer_H_


#include <asn_application.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Forward declarations */
struct CauseCode;
struct EventHistory;

/* SituationContainer */
typedef struct SituationContainer {
	InformationQuality_t	 informationQuality;
	CauseCode_t	 eventType;
	struct CauseCode	*linkedCause;	/* OPTIONAL */
	struct EventHistory	*eventHistory;	/* OPTIONAL */
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} SituationContainer_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_SituationContainer;
extern asn_SEQUENCE_specifics_t asn_SPC_SituationContainer_specs_1;
extern asn_TYPE_member_t asn_MBR_SituationContainer_1[4];

#ifdef __cplusplus
}
#endif

/* Referred external types */
#include "CauseCode.h"
#include "EventHistory.h"

#endif	/* _SituationContainer_H_ */
#include <asn_internal.h>
