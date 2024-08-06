/*
 * Generated by asn1c-0.9.29-DF (http://lionet.info/asn1c)
 * From ASN.1 module "Ieee1609Dot2CrlBaseTypes"
 * 	found in "asn1/ieee1609.2/Ieee1609Dot2CrlBaseTypes.asn"
 * 	`asn1c -fcompound-names -no-gen-BER -no-gen-JER -no-gen-UPER -no-gen-APER -no-gen-random-fill -no-gen-example -pdu=EtsiTs103097Data -pdu=EnrolmentRequestMessage -pdu=EnrolmentResponseMessage -pdu=AuthorizationRequestMessage -pdu=AuthorizationRequestMessageWithPop -pdu=AuthorizationResponseMessage -pdu=CertificateRevocationListMessage -pdu=TlmCertificateTrustListMessage -pdu=RcaCertificateTrustListMessage -pdu=AuthorizationValidationRequestMessage -pdu=AuthorizationValidationResponseMessage -pdu=CaCertificateRequestMessage -pdu=EtsiTs103097Certificate`
 */

#include "SequenceOfIMaxGroup.h"

asn_TYPE_member_t asn_MBR_SequenceOfIMaxGroup_1[] = {
	{ ATF_POINTER, 0, 0,
		(ASN_TAG_CLASS_UNIVERSAL | (16 << 2)),
		0,
		&asn_DEF_IMaxGroup,
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
		""
		},
};
static const ber_tlv_tag_t asn_DEF_SequenceOfIMaxGroup_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
asn_SET_OF_specifics_t asn_SPC_SequenceOfIMaxGroup_specs_1 = {
	sizeof(struct SequenceOfIMaxGroup),
	offsetof(struct SequenceOfIMaxGroup, _asn_ctx),
	0,	/* XER encoding is XMLDelimitedItemList */
};
asn_TYPE_descriptor_t asn_DEF_SequenceOfIMaxGroup = {
	"SequenceOfIMaxGroup",
	"SequenceOfIMaxGroup",
	&asn_OP_SEQUENCE_OF,
	asn_DEF_SequenceOfIMaxGroup_tags_1,
	sizeof(asn_DEF_SequenceOfIMaxGroup_tags_1)
		/sizeof(asn_DEF_SequenceOfIMaxGroup_tags_1[0]), /* 1 */
	asn_DEF_SequenceOfIMaxGroup_tags_1,	/* Same as above */
	sizeof(asn_DEF_SequenceOfIMaxGroup_tags_1)
		/sizeof(asn_DEF_SequenceOfIMaxGroup_tags_1[0]), /* 1 */
	{
#if !defined(ASN_DISABLE_OER_SUPPORT)
		0,
#endif  /* !defined(ASN_DISABLE_OER_SUPPORT) */
#if !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT)
		0,
#endif  /* !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT) */
		SEQUENCE_OF_constraint
	},
	asn_MBR_SequenceOfIMaxGroup_1,
	1,	/* Single element */
	&asn_SPC_SequenceOfIMaxGroup_specs_1	/* Additional specs */
};

