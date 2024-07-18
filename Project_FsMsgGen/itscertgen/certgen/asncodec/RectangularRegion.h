/*
 * Generated by asn1c-0.9.29-DF (http://lionet.info/asn1c)
 * From ASN.1 module "Ieee1609Dot2BaseTypes"
 * 	found in "asn1/ieee1609.2/IEEE1609dot2BaseTypes.asn"
 * 	`asn1c -S ../../../../asn1c-fillabs2/skeletons -fcompound-names -no-gen-BER -no-gen-JER -no-gen-UPER -no-gen-APER -no-gen-random-fill -no-gen-example -pdu=EtsiTs103097Data -pdu=EnrolmentRequestMessage -pdu=EnrolmentResponseMessage -pdu=AuthorizationRequestMessage -pdu=AuthorizationRequestMessageWithPop -pdu=AuthorizationResponseMessage -pdu=CertificateRevocationListMessage -pdu=TlmCertificateTrustListMessage -pdu=RcaCertificateTrustListMessage -pdu=AuthorizationValidationRequestMessage -pdu=AuthorizationValidationResponseMessage -pdu=CaCertificateRequestMessage -pdu=EtsiTs103097Certificate`
 */


/* Including external dependencies */
#include "TwoDLocation.h"
#include <constr_SEQUENCE.h>
#ifndef	_RectangularRegion_H_
#define	_RectangularRegion_H_


#include <asn_application.h>

#ifdef __cplusplus
extern "C" {
#endif

/* RectangularRegion */
typedef struct RectangularRegion {
	TwoDLocation_t	 northWest;
	TwoDLocation_t	 southEast;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} RectangularRegion_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_RectangularRegion;
extern asn_SEQUENCE_specifics_t asn_SPC_RectangularRegion_specs_1;
extern asn_TYPE_member_t asn_MBR_RectangularRegion_1[2];

#ifdef __cplusplus
}
#endif

#endif	/* _RectangularRegion_H_ */
#include <asn_internal.h>
