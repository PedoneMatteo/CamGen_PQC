/*
 * Generated by asn1c-0.9.29-DF (http://lionet.info/asn1c)
 * From ASN.1 module "Ieee1609Dot2Dot1AcaEeInterface"
 * 	found in "asn1/ieee1609.2.1/Ieee1609Dot2Dot1AcaEeInterface.asn"
 * 	`asn1c -S ../../../../asn1c-fillabs2/skeletons -fcompound-names -no-gen-BER -no-gen-JER -no-gen-UPER -no-gen-APER -no-gen-random-fill -no-gen-example -pdu=EtsiTs103097Data -pdu=EnrolmentRequestMessage -pdu=EnrolmentResponseMessage -pdu=AuthorizationRequestMessage -pdu=AuthorizationRequestMessageWithPop -pdu=AuthorizationResponseMessage -pdu=CertificateRevocationListMessage -pdu=TlmCertificateTrustListMessage -pdu=RcaCertificateTrustListMessage -pdu=AuthorizationValidationRequestMessage -pdu=AuthorizationValidationResponseMessage -pdu=CaCertificateRequestMessage -pdu=EtsiTs103097Certificate`
 */

#include "AcaEeInterfacePdu.h"

#if !defined(ASN_DISABLE_OER_SUPPORT)
static asn_oer_constraints_t asn_OER_type_AcaEeInterfacePdu_constr_1 CC_NOTUSED = {
	{ 0, 0 },
	-1};
#endif  /* !defined(ASN_DISABLE_OER_SUPPORT) */
asn_TYPE_member_t asn_MBR_AcaEeInterfacePdu_1[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct AcaEeInterfacePdu, choice.acaEeCertResponse),
		(ASN_TAG_CLASS_CONTEXT | (0 << 2)),
		-1,	/* IMPLICIT tag at current level */
		&asn_DEF_AcaEeCertResponse,
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
		"acaEeCertResponse"
		},
};
static const asn_TYPE_tag2member_t asn_MAP_AcaEeInterfacePdu_tag2el_1[] = {
    { (ASN_TAG_CLASS_CONTEXT | (0 << 2)), 0, 0, 0 } /* acaEeCertResponse */
};
asn_CHOICE_specifics_t asn_SPC_AcaEeInterfacePdu_specs_1 = {
	sizeof(struct AcaEeInterfacePdu),
	offsetof(struct AcaEeInterfacePdu, _asn_ctx),
	offsetof(struct AcaEeInterfacePdu, present),
	sizeof(((struct AcaEeInterfacePdu *)0)->present),
	asn_MAP_AcaEeInterfacePdu_tag2el_1,
	1,	/* Count of tags in the map */
	0, 0,
	1	/* Extensions start */
};
asn_TYPE_descriptor_t asn_DEF_AcaEeInterfacePdu = {
	"AcaEeInterfacePdu",
	"AcaEeInterfacePdu",
	&asn_OP_CHOICE,
	0,	/* No effective tags (pointer) */
	0,	/* No effective tags (count) */
	0,	/* No tags (pointer) */
	0,	/* No tags (count) */
	{
#if !defined(ASN_DISABLE_OER_SUPPORT)
		&asn_OER_type_AcaEeInterfacePdu_constr_1,
#endif  /* !defined(ASN_DISABLE_OER_SUPPORT) */
#if !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT)
		0,
#endif  /* !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT) */
		CHOICE_constraint
	},
	asn_MBR_AcaEeInterfacePdu_1,
	1,	/* Elements count */
	&asn_SPC_AcaEeInterfacePdu_specs_1	/* Additional specs */
};
