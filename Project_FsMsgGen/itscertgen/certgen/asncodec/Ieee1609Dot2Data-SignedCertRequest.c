/*
 * Generated by asn1c-0.9.29-DF (http://lionet.info/asn1c)
 * From ASN.1 module "Ieee1609Dot2Dot1Protocol"
 * 	found in "asn1/ieee1609.2.1/Ieee1609Dot2Dot1Protocol.asn"
 * 	`asn1c -fcompound-names -no-gen-BER -no-gen-JER -no-gen-UPER -no-gen-APER -no-gen-random-fill -no-gen-example -pdu=EtsiTs103097Data -pdu=EnrolmentRequestMessage -pdu=EnrolmentResponseMessage -pdu=AuthorizationRequestMessage -pdu=AuthorizationRequestMessageWithPop -pdu=AuthorizationResponseMessage -pdu=CertificateRevocationListMessage -pdu=TlmCertificateTrustListMessage -pdu=RcaCertificateTrustListMessage -pdu=AuthorizationValidationRequestMessage -pdu=AuthorizationValidationResponseMessage -pdu=CaCertificateRequestMessage -pdu=EtsiTs103097Certificate`
 */

#include "Ieee1609Dot2Data-SignedCertRequest.h"

int
Ieee1609Dot2Data_SignedCertRequest_290P0_constraint(const asn_TYPE_descriptor_t *td, const void *sptr,
			asn_app_constraint_failed_f *ctfailcb, void *app_key) {
	
	if(!sptr) {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: value not given (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
	
	
	if(1 /* No applicable constraints whatsoever */) {
		/* Nothing is here. See below */
	}
	
	return td->encoding_constraints.general_constraints(td, sptr, ctfailcb, app_key);
}

/*
 * This type is implemented using Ieee1609Dot2Data,
 * so here we adjust the DEF accordingly.
 */
int
Ieee1609Dot2Data_SignedCertRequest_290P1_constraint(const asn_TYPE_descriptor_t *td, const void *sptr,
			asn_app_constraint_failed_f *ctfailcb, void *app_key) {
	
	if(!sptr) {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: value not given (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
	
	
	if(1 /* No applicable constraints whatsoever */) {
		/* Nothing is here. See below */
	}
	
	return td->encoding_constraints.general_constraints(td, sptr, ctfailcb, app_key);
}

/*
 * This type is implemented using Ieee1609Dot2Data,
 * so here we adjust the DEF accordingly.
 */
#if !defined(ASN_DISABLE_OER_SUPPORT)
static asn_oer_constraints_t asn_OER_type_Ieee1609Dot2Data_SignedCertRequest_290P0_constr_1 CC_NOTUSED = {
	{ 0, 0 },
	-1};
#endif  /* !defined(ASN_DISABLE_OER_SUPPORT) */
#if !defined(ASN_DISABLE_OER_SUPPORT)
static asn_oer_constraints_t asn_OER_type_Ieee1609Dot2Data_SignedCertRequest_290P1_constr_2 CC_NOTUSED = {
	{ 0, 0 },
	-1};
#endif  /* !defined(ASN_DISABLE_OER_SUPPORT) */
static const ber_tlv_tag_t asn_DEF_Ieee1609Dot2Data_SignedCertRequest_290P0_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
asn_TYPE_descriptor_t asn_DEF_Ieee1609Dot2Data_SignedCertRequest_290P0 = {
	"Ieee1609Dot2Data-SignedCertRequest",
	"Ieee1609Dot2Data-SignedCertRequest",
	&asn_OP_SEQUENCE,
	asn_DEF_Ieee1609Dot2Data_SignedCertRequest_290P0_tags_1,
	sizeof(asn_DEF_Ieee1609Dot2Data_SignedCertRequest_290P0_tags_1)
		/sizeof(asn_DEF_Ieee1609Dot2Data_SignedCertRequest_290P0_tags_1[0]), /* 1 */
	asn_DEF_Ieee1609Dot2Data_SignedCertRequest_290P0_tags_1,	/* Same as above */
	sizeof(asn_DEF_Ieee1609Dot2Data_SignedCertRequest_290P0_tags_1)
		/sizeof(asn_DEF_Ieee1609Dot2Data_SignedCertRequest_290P0_tags_1[0]), /* 1 */
	{
#if !defined(ASN_DISABLE_OER_SUPPORT)
		&asn_OER_type_Ieee1609Dot2Data_SignedCertRequest_290P0_constr_1,
#endif  /* !defined(ASN_DISABLE_OER_SUPPORT) */
#if !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT)
		0,
#endif  /* !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT) */
		Ieee1609Dot2Data_SignedCertRequest_290P0_constraint
	},
	asn_MBR_Ieee1609Dot2Data_1,
	2,	/* Elements count */
	&asn_SPC_Ieee1609Dot2Data_specs_1	/* Additional specs */
};

static const ber_tlv_tag_t asn_DEF_Ieee1609Dot2Data_SignedCertRequest_290P1_tags_2[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
asn_TYPE_descriptor_t asn_DEF_Ieee1609Dot2Data_SignedCertRequest_290P1 = {
	"Ieee1609Dot2Data-SignedCertRequest",
	"Ieee1609Dot2Data-SignedCertRequest",
	&asn_OP_SEQUENCE,
	asn_DEF_Ieee1609Dot2Data_SignedCertRequest_290P1_tags_2,
	sizeof(asn_DEF_Ieee1609Dot2Data_SignedCertRequest_290P1_tags_2)
		/sizeof(asn_DEF_Ieee1609Dot2Data_SignedCertRequest_290P1_tags_2[0]), /* 1 */
	asn_DEF_Ieee1609Dot2Data_SignedCertRequest_290P1_tags_2,	/* Same as above */
	sizeof(asn_DEF_Ieee1609Dot2Data_SignedCertRequest_290P1_tags_2)
		/sizeof(asn_DEF_Ieee1609Dot2Data_SignedCertRequest_290P1_tags_2[0]), /* 1 */
	{
#if !defined(ASN_DISABLE_OER_SUPPORT)
		&asn_OER_type_Ieee1609Dot2Data_SignedCertRequest_290P1_constr_2,
#endif  /* !defined(ASN_DISABLE_OER_SUPPORT) */
#if !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT)
		0,
#endif  /* !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT) */
		Ieee1609Dot2Data_SignedCertRequest_290P1_constraint
	},
	asn_MBR_Ieee1609Dot2Data_1,
	2,	/* Elements count */
	&asn_SPC_Ieee1609Dot2Data_specs_1	/* Additional specs */
};

