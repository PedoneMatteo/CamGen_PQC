/*
 * Generated by asn1c-0.9.29-DF (http://lionet.info/asn1c)
 * From ASN.1 module "EtsiTs102941TrustLists"
 * 	found in "asn1/pki_ts102941/EtsiTs102941TrustLists.asn"
 * 	`asn1c -S ../../../../asn1c-fillabs2/skeletons -fcompound-names -no-gen-BER -no-gen-JER -no-gen-UPER -no-gen-APER -no-gen-random-fill -no-gen-example -pdu=EtsiTs103097Data -pdu=EnrolmentRequestMessage -pdu=EnrolmentResponseMessage -pdu=AuthorizationRequestMessage -pdu=AuthorizationRequestMessageWithPop -pdu=AuthorizationResponseMessage -pdu=CertificateRevocationListMessage -pdu=TlmCertificateTrustListMessage -pdu=RcaCertificateTrustListMessage -pdu=AuthorizationValidationRequestMessage -pdu=AuthorizationValidationResponseMessage -pdu=CaCertificateRequestMessage -pdu=EtsiTs103097Certificate`
 */


/* Including external dependencies */
#include "Url.h"
#include "HashedId8.h"
#include <asn_SEQUENCE_OF.h>
#include <constr_SEQUENCE_OF.h>
#include <constr_SEQUENCE.h>
#ifndef	_DcEntry_H_
#define	_DcEntry_H_


#include <asn_application.h>

#ifdef __cplusplus
extern "C" {
#endif

/* DcEntry */
typedef struct DcEntry {
	Url_t	 url;
	struct DcEntry__cert {
		A_SEQUENCE_OF(HashedId8_t) list;
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} cert;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} DcEntry_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_DcEntry;
extern asn_SEQUENCE_specifics_t asn_SPC_DcEntry_specs_1;
extern asn_TYPE_member_t asn_MBR_DcEntry_1[2];

#ifdef __cplusplus
}
#endif

#endif	/* _DcEntry_H_ */
#include <asn_internal.h>