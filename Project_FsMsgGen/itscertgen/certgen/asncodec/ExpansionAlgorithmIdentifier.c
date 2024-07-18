/*
 * Generated by asn1c-0.9.29-DF (http://lionet.info/asn1c)
 * From ASN.1 module "Ieee1609Dot2CrlBaseTypes"
 * 	found in "asn1/ieee1609.2/Ieee1609Dot2CrlBaseTypes.asn"
 * 	`asn1c -S ../../../../asn1c-fillabs2/skeletons -fcompound-names -no-gen-BER -no-gen-JER -no-gen-UPER -no-gen-APER -no-gen-random-fill -no-gen-example -pdu=EtsiTs103097Data -pdu=EnrolmentRequestMessage -pdu=EnrolmentResponseMessage -pdu=AuthorizationRequestMessage -pdu=AuthorizationRequestMessageWithPop -pdu=AuthorizationResponseMessage -pdu=CertificateRevocationListMessage -pdu=TlmCertificateTrustListMessage -pdu=RcaCertificateTrustListMessage -pdu=AuthorizationValidationRequestMessage -pdu=AuthorizationValidationResponseMessage -pdu=CaCertificateRequestMessage -pdu=EtsiTs103097Certificate`
 */

#include "ExpansionAlgorithmIdentifier.h"

/*
 * This type is implemented using NativeEnumerated,
 * so here we adjust the DEF accordingly.
 */
#if !defined(ASN_DISABLE_OER_SUPPORT)
static asn_oer_constraints_t asn_OER_type_ExpansionAlgorithmIdentifier_constr_1 CC_NOTUSED = {
	{ 0, 0 },
	-1};
#endif  /* !defined(ASN_DISABLE_OER_SUPPORT) */
static const asn_INTEGER_enum_map_t asn_MAP_ExpansionAlgorithmIdentifier_value2enum_1[] = {
	{ 0,	18,	"sha256ForI-aesForJ" },
	{ 1,	15,	"sm3ForI-sm4ForJ" }
	/* This list is extensible */
};
static const unsigned int asn_MAP_ExpansionAlgorithmIdentifier_enum2value_1[] = {
	0,	/* sha256ForI-aesForJ(0) */
	1	/* sm3ForI-sm4ForJ(1) */
	/* This list is extensible */
};
static const asn_INTEGER_specifics_t asn_SPC_ExpansionAlgorithmIdentifier_specs_1 = {
	asn_MAP_ExpansionAlgorithmIdentifier_value2enum_1,	/* "tag" => N; sorted by tag */
	asn_MAP_ExpansionAlgorithmIdentifier_enum2value_1,	/* N => "tag"; sorted by N */
	2,	/* Number of elements in the maps */
	3,	/* Extensions before this member */
	1,	/* Strict enumeration */
	0,	/* Native long size */
	0
};
static const ber_tlv_tag_t asn_DEF_ExpansionAlgorithmIdentifier_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (10 << 2))
};
asn_TYPE_descriptor_t asn_DEF_ExpansionAlgorithmIdentifier = {
	"ExpansionAlgorithmIdentifier",
	"ExpansionAlgorithmIdentifier",
	&asn_OP_NativeEnumerated,
	asn_DEF_ExpansionAlgorithmIdentifier_tags_1,
	sizeof(asn_DEF_ExpansionAlgorithmIdentifier_tags_1)
		/sizeof(asn_DEF_ExpansionAlgorithmIdentifier_tags_1[0]), /* 1 */
	asn_DEF_ExpansionAlgorithmIdentifier_tags_1,	/* Same as above */
	sizeof(asn_DEF_ExpansionAlgorithmIdentifier_tags_1)
		/sizeof(asn_DEF_ExpansionAlgorithmIdentifier_tags_1[0]), /* 1 */
	{
#if !defined(ASN_DISABLE_OER_SUPPORT)
		&asn_OER_type_ExpansionAlgorithmIdentifier_constr_1,
#endif  /* !defined(ASN_DISABLE_OER_SUPPORT) */
#if !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT)
		0,
#endif  /* !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT) */
		NativeEnumerated_constraint
	},
	0, 0,	/* Defined elsewhere */
	&asn_SPC_ExpansionAlgorithmIdentifier_specs_1	/* Additional specs */
};

