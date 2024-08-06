/*
 * Generated by asn1c-0.9.29-DF (http://lionet.info/asn1c)
 * From ASN.1 module "Ieee1609Dot2CrlBaseTypes"
 * 	found in "asn1/ieee1609.2/Ieee1609Dot2CrlBaseTypes.asn"
 * 	`asn1c -fcompound-names -no-gen-BER -no-gen-JER -no-gen-UPER -no-gen-APER -no-gen-random-fill -no-gen-example -pdu=EtsiTs103097Data -pdu=EnrolmentRequestMessage -pdu=EnrolmentResponseMessage -pdu=AuthorizationRequestMessage -pdu=AuthorizationRequestMessageWithPop -pdu=AuthorizationResponseMessage -pdu=CertificateRevocationListMessage -pdu=TlmCertificateTrustListMessage -pdu=RcaCertificateTrustListMessage -pdu=AuthorizationValidationRequestMessage -pdu=AuthorizationValidationResponseMessage -pdu=CaCertificateRequestMessage -pdu=EtsiTs103097Certificate`
 */

#include "CrlPriorityInfo.h"

asn_TYPE_member_t asn_MBR_CrlPriorityInfo_1[] = {
	{ ATF_POINTER, 1, offsetof(struct CrlPriorityInfo, priority),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_Uint8,
		0,
		{
#if !defined(ASN_DISABLE_OER_SUPPORT)
			0,
#endif  /* !defined(ASN_DISABLE_OER_SUPPORT) */
#if !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT)
			0,
#endif  /* !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT) */
			0
		},
		0, 0, /* No default value */
		"priority"
		},
};
static const int asn_MAP_CrlPriorityInfo_oms_1[] = { 0 };
static const ber_tlv_tag_t asn_DEF_CrlPriorityInfo_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_CrlPriorityInfo_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 } /* priority */
};
asn_SEQUENCE_specifics_t asn_SPC_CrlPriorityInfo_specs_1 = {
	sizeof(struct CrlPriorityInfo),
	offsetof(struct CrlPriorityInfo, _asn_ctx),
	asn_MAP_CrlPriorityInfo_tag2el_1,
	1,	/* Count of tags in the map */
	asn_MAP_CrlPriorityInfo_oms_1,	/* Optional members */
	1, 0,	/* Root/Additions */
	1,	/* First extension addition */
};
asn_TYPE_descriptor_t asn_DEF_CrlPriorityInfo = {
	"CrlPriorityInfo",
	"CrlPriorityInfo",
	&asn_OP_SEQUENCE,
	asn_DEF_CrlPriorityInfo_tags_1,
	sizeof(asn_DEF_CrlPriorityInfo_tags_1)
		/sizeof(asn_DEF_CrlPriorityInfo_tags_1[0]), /* 1 */
	asn_DEF_CrlPriorityInfo_tags_1,	/* Same as above */
	sizeof(asn_DEF_CrlPriorityInfo_tags_1)
		/sizeof(asn_DEF_CrlPriorityInfo_tags_1[0]), /* 1 */
	{
#if !defined(ASN_DISABLE_OER_SUPPORT)
		0,
#endif  /* !defined(ASN_DISABLE_OER_SUPPORT) */
#if !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT)
		0,
#endif  /* !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT) */
		SEQUENCE_constraint
	},
	asn_MBR_CrlPriorityInfo_1,
	1,	/* Elements count */
	&asn_SPC_CrlPriorityInfo_specs_1	/* Additional specs */
};

