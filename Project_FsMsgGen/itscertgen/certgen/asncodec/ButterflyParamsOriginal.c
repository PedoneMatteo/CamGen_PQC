/*
 * Generated by asn1c-0.9.29-DF (http://lionet.info/asn1c)
 * From ASN.1 module "Ieee1609Dot2Dot1EeRaInterface"
 * 	found in "asn1/ieee1609.2.1/Ieee1609Dot2Dot1EeRaInterface.asn"
 * 	`asn1c -S ../../../../asn1c-fillabs2/skeletons -fcompound-names -no-gen-BER -no-gen-JER -no-gen-UPER -no-gen-APER -no-gen-random-fill -no-gen-example -pdu=EtsiTs103097Data -pdu=EnrolmentRequestMessage -pdu=EnrolmentResponseMessage -pdu=AuthorizationRequestMessage -pdu=AuthorizationRequestMessageWithPop -pdu=AuthorizationResponseMessage -pdu=CertificateRevocationListMessage -pdu=TlmCertificateTrustListMessage -pdu=RcaCertificateTrustListMessage -pdu=AuthorizationValidationRequestMessage -pdu=AuthorizationValidationResponseMessage -pdu=CaCertificateRequestMessage -pdu=EtsiTs103097Certificate`
 */

#include "ButterflyParamsOriginal.h"

asn_TYPE_member_t asn_MBR_ButterflyParamsOriginal_1[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct ButterflyParamsOriginal, signingExpansion),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		+1,	/* EXPLICIT tag at current level */
		&asn_DEF_ButterflyExpansion,
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
		"signingExpansion"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct ButterflyParamsOriginal, encryptionKey),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_PublicEncryptionKey,
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
		"encryptionKey"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct ButterflyParamsOriginal, encryptionExpansion),
		(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
		+1,	/* EXPLICIT tag at current level */
		&asn_DEF_ButterflyExpansion,
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
		"encryptionExpansion"
		},
};
static const ber_tlv_tag_t asn_DEF_ButterflyParamsOriginal_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_ButterflyParamsOriginal_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* signingExpansion */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 }, /* encryptionKey */
    { (ASN_TAG_CLASS_CONTEXT | (2 << 2)), 2, 0, 0 } /* encryptionExpansion */
};
asn_SEQUENCE_specifics_t asn_SPC_ButterflyParamsOriginal_specs_1 = {
	sizeof(struct ButterflyParamsOriginal),
	offsetof(struct ButterflyParamsOriginal, _asn_ctx),
	asn_MAP_ButterflyParamsOriginal_tag2el_1,
	3,	/* Count of tags in the map */
	0, 0, 0,	/* Optional elements (not needed) */
	-1,	/* First extension addition */
};
asn_TYPE_descriptor_t asn_DEF_ButterflyParamsOriginal = {
	"ButterflyParamsOriginal",
	"ButterflyParamsOriginal",
	&asn_OP_SEQUENCE,
	asn_DEF_ButterflyParamsOriginal_tags_1,
	sizeof(asn_DEF_ButterflyParamsOriginal_tags_1)
		/sizeof(asn_DEF_ButterflyParamsOriginal_tags_1[0]), /* 1 */
	asn_DEF_ButterflyParamsOriginal_tags_1,	/* Same as above */
	sizeof(asn_DEF_ButterflyParamsOriginal_tags_1)
		/sizeof(asn_DEF_ButterflyParamsOriginal_tags_1[0]), /* 1 */
	{
#if !defined(ASN_DISABLE_OER_SUPPORT)
		0,
#endif  /* !defined(ASN_DISABLE_OER_SUPPORT) */
#if !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT)
		0,
#endif  /* !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT) */
		SEQUENCE_constraint
	},
	asn_MBR_ButterflyParamsOriginal_1,
	3,	/* Elements count */
	&asn_SPC_ButterflyParamsOriginal_specs_1	/* Additional specs */
};

