/*
 * Generated by asn1c-0.9.29-DF (http://lionet.info/asn1c)
 * From ASN.1 module "Ieee1609Dot2CrlBaseTypes"
 * 	found in "asn1/ieee1609.2/Ieee1609Dot2CrlBaseTypes.asn"
 * 	`asn1c -fcompound-names -no-gen-BER -no-gen-JER -no-gen-UPER -no-gen-APER -no-gen-random-fill -no-gen-example -pdu=EtsiTs103097Data -pdu=EnrolmentRequestMessage -pdu=EnrolmentResponseMessage -pdu=AuthorizationRequestMessage -pdu=AuthorizationRequestMessageWithPop -pdu=AuthorizationResponseMessage -pdu=CertificateRevocationListMessage -pdu=TlmCertificateTrustListMessage -pdu=RcaCertificateTrustListMessage -pdu=AuthorizationValidationRequestMessage -pdu=AuthorizationValidationResponseMessage -pdu=CaCertificateRequestMessage -pdu=EtsiTs103097Certificate`
 */

#include "ToBeSignedLinkageValueCrlWithAlgIdentifier.h"

#if !defined(ASN_DISABLE_OER_SUPPORT)
static asn_oer_constraints_t asn_OER_type_ToBeSignedLinkageValueCrlWithAlgIdentifier_constr_1 CC_NOTUSED = {
	{ 0, 0 },
	-1};
#endif  /* !defined(ASN_DISABLE_OER_SUPPORT) */
asn_TYPE_member_t asn_MBR_ToBeSignedLinkageValueCrlWithAlgIdentifier_1[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct ToBeSignedLinkageValueCrlWithAlgIdentifier, iRev),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_IValue,
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
		"iRev"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct ToBeSignedLinkageValueCrlWithAlgIdentifier, indexWithinI),
		(ASN_TAG_CLASS_CONTEXT | (1 << 2)),
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
		"indexWithinI"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct ToBeSignedLinkageValueCrlWithAlgIdentifier, seedEvolution),
		(ASN_TAG_CLASS_CONTEXT | (2 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_SeedEvolutionFunctionIdentifier,
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
		"seedEvolution"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct ToBeSignedLinkageValueCrlWithAlgIdentifier, lvGeneration),
		(ASN_TAG_CLASS_CONTEXT | (3 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_LvGenerationFunctionIdentifier,
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
		"lvGeneration"
		},
	{ ATF_POINTER, 3, offsetof(struct ToBeSignedLinkageValueCrlWithAlgIdentifier, individual),
		(ASN_TAG_CLASS_CONTEXT | (4 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_SequenceOfJMaxGroup,
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
		"individual"
		},
	{ ATF_POINTER, 2, offsetof(struct ToBeSignedLinkageValueCrlWithAlgIdentifier, groups),
		(ASN_TAG_CLASS_CONTEXT | (5 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_SequenceOfGroupCrlEntry,
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
		"groups"
		},
	{ ATF_POINTER, 1, offsetof(struct ToBeSignedLinkageValueCrlWithAlgIdentifier, groupsSingleSeed),
		(ASN_TAG_CLASS_CONTEXT | (6 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_SequenceOfGroupSingleSeedCrlEntry,
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
		"groupsSingleSeed"
		},
};
static const int asn_MAP_ToBeSignedLinkageValueCrlWithAlgIdentifier_oms_1[] = { 4, 5, 6 };
static const ber_tlv_tag_t asn_DEF_ToBeSignedLinkageValueCrlWithAlgIdentifier_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_ToBeSignedLinkageValueCrlWithAlgIdentifier_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 }, /* iRev */
    { (ASN_TAG_CLASS_CONTEXT | (1 << 2)), 1, 0, 0 }, /* indexWithinI */
    { (ASN_TAG_CLASS_CONTEXT | (2 << 2)), 2, 0, 0 }, /* seedEvolution */
    { (ASN_TAG_CLASS_CONTEXT | (3 << 2)), 3, 0, 0 }, /* lvGeneration */
    { (ASN_TAG_CLASS_CONTEXT | (4 << 2)), 4, 0, 0 }, /* individual */
    { (ASN_TAG_CLASS_CONTEXT | (5 << 2)), 5, 0, 0 }, /* groups */
    { (ASN_TAG_CLASS_CONTEXT | (6 << 2)), 6, 0, 0 } /* groupsSingleSeed */
};
asn_SEQUENCE_specifics_t asn_SPC_ToBeSignedLinkageValueCrlWithAlgIdentifier_specs_1 = {
	sizeof(struct ToBeSignedLinkageValueCrlWithAlgIdentifier),
	offsetof(struct ToBeSignedLinkageValueCrlWithAlgIdentifier, _asn_ctx),
	asn_MAP_ToBeSignedLinkageValueCrlWithAlgIdentifier_tag2el_1,
	7,	/* Count of tags in the map */
	asn_MAP_ToBeSignedLinkageValueCrlWithAlgIdentifier_oms_1,	/* Optional members */
	3, 0,	/* Root/Additions */
	7,	/* First extension addition */
};
asn_TYPE_descriptor_t asn_DEF_ToBeSignedLinkageValueCrlWithAlgIdentifier = {
	"ToBeSignedLinkageValueCrlWithAlgIdentifier",
	"ToBeSignedLinkageValueCrlWithAlgIdentifier",
	&asn_OP_SEQUENCE,
	asn_DEF_ToBeSignedLinkageValueCrlWithAlgIdentifier_tags_1,
	sizeof(asn_DEF_ToBeSignedLinkageValueCrlWithAlgIdentifier_tags_1)
		/sizeof(asn_DEF_ToBeSignedLinkageValueCrlWithAlgIdentifier_tags_1[0]), /* 1 */
	asn_DEF_ToBeSignedLinkageValueCrlWithAlgIdentifier_tags_1,	/* Same as above */
	sizeof(asn_DEF_ToBeSignedLinkageValueCrlWithAlgIdentifier_tags_1)
		/sizeof(asn_DEF_ToBeSignedLinkageValueCrlWithAlgIdentifier_tags_1[0]), /* 1 */
	{
#if !defined(ASN_DISABLE_OER_SUPPORT)
		&asn_OER_type_ToBeSignedLinkageValueCrlWithAlgIdentifier_constr_1,
#endif  /* !defined(ASN_DISABLE_OER_SUPPORT) */
#if !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT)
		0,
#endif  /* !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT) */
		SEQUENCE_constraint
	},
	asn_MBR_ToBeSignedLinkageValueCrlWithAlgIdentifier_1,
	7,	/* Elements count */
	&asn_SPC_ToBeSignedLinkageValueCrlWithAlgIdentifier_specs_1	/* Additional specs */
};

