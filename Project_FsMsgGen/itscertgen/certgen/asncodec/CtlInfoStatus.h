/*
 * Generated by asn1c-0.9.29-DF (http://lionet.info/asn1c)
 * From ASN.1 module "Ieee1609Dot2Dot1CertManagement"
 * 	found in "asn1/ieee1609.2.1/Ieee1609Dot2Dot1CertManagement.asn"
 * 	`asn1c -fcompound-names -no-gen-BER -no-gen-JER -no-gen-UPER -no-gen-APER -no-gen-random-fill -no-gen-example -pdu=EtsiTs103097Data -pdu=EnrolmentRequestMessage -pdu=EnrolmentResponseMessage -pdu=AuthorizationRequestMessage -pdu=AuthorizationRequestMessageWithPop -pdu=AuthorizationResponseMessage -pdu=CertificateRevocationListMessage -pdu=TlmCertificateTrustListMessage -pdu=RcaCertificateTrustListMessage -pdu=AuthorizationValidationRequestMessage -pdu=AuthorizationValidationResponseMessage -pdu=CaCertificateRequestMessage -pdu=EtsiTs103097Certificate`
 */


/* Including external dependencies */
#include "CtlSeriesId.h"
#include "CtlSequenceNumber.h"
#include "Time32.h"
#include <constr_SEQUENCE.h>
#ifndef	_CtlInfoStatus_H_
#define	_CtlInfoStatus_H_


#include <asn_application.h>

#ifdef __cplusplus
extern "C" {
#endif

/* CtlInfoStatus */
typedef struct CtlInfoStatus {
	CtlSeriesId_t	 ctlSeriesId;
	CtlSequenceNumber_t	 sequenceNumber;
	Time32_t	 lastUpdate;
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} CtlInfoStatus_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_CtlInfoStatus;
extern asn_SEQUENCE_specifics_t asn_SPC_CtlInfoStatus_specs_1;
extern asn_TYPE_member_t asn_MBR_CtlInfoStatus_1[3];

#ifdef __cplusplus
}
#endif

#endif	/* _CtlInfoStatus_H_ */
#include <asn_internal.h>
