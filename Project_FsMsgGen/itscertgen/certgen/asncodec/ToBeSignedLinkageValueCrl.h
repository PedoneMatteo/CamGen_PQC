/*
 * Generated by asn1c-0.9.29-DF (http://lionet.info/asn1c)
 * From ASN.1 module "Ieee1609Dot2CrlBaseTypes"
 * 	found in "asn1/ieee1609.2/Ieee1609Dot2CrlBaseTypes.asn"
 * 	`asn1c -fcompound-names -no-gen-BER -no-gen-JER -no-gen-UPER -no-gen-APER -no-gen-random-fill -no-gen-example -pdu=EtsiTs103097Data -pdu=EnrolmentRequestMessage -pdu=EnrolmentResponseMessage -pdu=AuthorizationRequestMessage -pdu=AuthorizationRequestMessageWithPop -pdu=AuthorizationResponseMessage -pdu=CertificateRevocationListMessage -pdu=TlmCertificateTrustListMessage -pdu=RcaCertificateTrustListMessage -pdu=AuthorizationValidationRequestMessage -pdu=AuthorizationValidationResponseMessage -pdu=CaCertificateRequestMessage -pdu=EtsiTs103097Certificate`
 */


/* Including external dependencies */
#include "IValue.h"
#include "Uint8.h"
#include <constr_SEQUENCE.h>
#ifndef	_ToBeSignedLinkageValueCrl_H_
#define	_ToBeSignedLinkageValueCrl_H_


#include <asn_application.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Forward declarations */
struct SequenceOfJMaxGroup;
struct SequenceOfGroupCrlEntry;
struct SequenceOfGroupSingleSeedCrlEntry;

/* ToBeSignedLinkageValueCrl */
typedef struct ToBeSignedLinkageValueCrl {
	IValue_t	 iRev;
	Uint8_t	 indexWithinI;
	struct SequenceOfJMaxGroup	*individual;	/* OPTIONAL */
	struct SequenceOfGroupCrlEntry	*groups;	/* OPTIONAL */
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	struct SequenceOfGroupSingleSeedCrlEntry	*groupsSingleSeed;	/* OPTIONAL */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} ToBeSignedLinkageValueCrl_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_ToBeSignedLinkageValueCrl;
extern asn_SEQUENCE_specifics_t asn_SPC_ToBeSignedLinkageValueCrl_specs_1;
extern asn_TYPE_member_t asn_MBR_ToBeSignedLinkageValueCrl_1[5];

#ifdef __cplusplus
}
#endif

/* Referred external types */
#include "SequenceOfJMaxGroup.h"
#include "SequenceOfGroupCrlEntry.h"
#include "SequenceOfGroupSingleSeedCrlEntry.h"

#endif	/* _ToBeSignedLinkageValueCrl_H_ */
#include <asn_internal.h>
