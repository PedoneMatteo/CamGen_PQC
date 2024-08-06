/*
 * Generated by asn1c-0.9.29-DF (http://lionet.info/asn1c)
 * From ASN.1 module "Ieee1609Dot2Dot1CertManagement"
 * 	found in "asn1/ieee1609.2.1/Ieee1609Dot2Dot1CertManagement.asn"
 * 	`asn1c -fcompound-names -no-gen-BER -no-gen-JER -no-gen-UPER -no-gen-APER -no-gen-random-fill -no-gen-example -pdu=EtsiTs103097Data -pdu=EnrolmentRequestMessage -pdu=EnrolmentResponseMessage -pdu=AuthorizationRequestMessage -pdu=AuthorizationRequestMessageWithPop -pdu=AuthorizationResponseMessage -pdu=CertificateRevocationListMessage -pdu=TlmCertificateTrustListMessage -pdu=RcaCertificateTrustListMessage -pdu=AuthorizationValidationRequestMessage -pdu=AuthorizationValidationResponseMessage -pdu=CaCertificateRequestMessage -pdu=EtsiTs103097Certificate`
 */


/* Including external dependencies */
#include "Ieee1609dot2dot1MsctlType.h"
#include <ANY.h>
#include <asn_ioc.h>
#include "FullIeeeTbsCtl.h"
#include "SequenceOfCertificate.h"
#include <OPEN_TYPE.h>
#include <constr_CHOICE.h>
#include <asn_SEQUENCE_OF.h>
#include <constr_SEQUENCE_OF.h>
#include <constr_SEQUENCE.h>
#ifndef	_MultiSignedCtl_H_
#define	_MultiSignedCtl_H_


#include <asn_application.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum MultiSignedCtl__tbsCtl_PR {
	MultiSignedCtl__tbsCtl_PR_NOTHING,	/* No components present */
	MultiSignedCtl__tbsCtl_PR_FullIeeeTbsCtl
} MultiSignedCtl__tbsCtl_PR;
typedef enum MultiSignedCtl__unsigned_PR {
	MultiSignedCtl__unsigned_PR_NOTHING,	/* No components present */
	MultiSignedCtl__unsigned_PR_SequenceOfCertificate
} MultiSignedCtl__unsigned_PR;

/* Forward declarations */
struct CtlSignatureSpdu;

/* MultiSignedCtl */
typedef struct MultiSignedCtl {
	Ieee1609dot2dot1MsctlType_t	 type;
	struct MultiSignedCtl__tbsCtl {
		MultiSignedCtl__tbsCtl_PR present;
		union MultiSignedCtl__tbsCtl_u {
			FullIeeeTbsCtl_t	 FullIeeeTbsCtl;
		} choice;
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} tbsCtl;
	struct MultiSignedCtl__unsigned {
		MultiSignedCtl__unsigned_PR present;
		union MultiSignedCtl__unsigned_u {
			SequenceOfCertificate_t	 SequenceOfCertificate;
		} choice;
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} Unsigned;
	struct MultiSignedCtl__signatures {
		A_SEQUENCE_OF(struct CtlSignatureSpdu) list;
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} signatures;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} MultiSignedCtl_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_MultiSignedCtl;
extern asn_SEQUENCE_specifics_t asn_SPC_MultiSignedCtl_specs_1;
extern asn_TYPE_member_t asn_MBR_MultiSignedCtl_1[4];

#ifdef __cplusplus
}
#endif

/* Referred external types */
#include "CtlSignatureSpdu.h"

#endif	/* _MultiSignedCtl_H_ */
#include <asn_internal.h>
