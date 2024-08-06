/*
 * Generated by asn1c-0.9.29-DF (http://lionet.info/asn1c)
 * From ASN.1 module "Ieee1609Dot2Dot1MaRaInterface"
 * 	found in "asn1/ieee1609.2.1/Ieee1609Dot2Dot1MaRaInterface.asn"
 * 	`asn1c -fcompound-names -no-gen-BER -no-gen-JER -no-gen-UPER -no-gen-APER -no-gen-random-fill -no-gen-example -pdu=EtsiTs103097Data -pdu=EnrolmentRequestMessage -pdu=EnrolmentResponseMessage -pdu=AuthorizationRequestMessage -pdu=AuthorizationRequestMessageWithPop -pdu=AuthorizationResponseMessage -pdu=CertificateRevocationListMessage -pdu=TlmCertificateTrustListMessage -pdu=RcaCertificateTrustListMessage -pdu=AuthorizationValidationRequestMessage -pdu=AuthorizationValidationResponseMessage -pdu=CaCertificateRequestMessage -pdu=EtsiTs103097Certificate`
 */

#include "MaRaInterfacePdu.h"

/*
 * This type is implemented using NULL,
 * so here we adjust the DEF accordingly.
 */
static const ber_tlv_tag_t asn_DEF_MaRaInterfacePdu_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (5 << 2))
};
asn_TYPE_descriptor_t asn_DEF_MaRaInterfacePdu = {
	"MaRaInterfacePdu",
	"MaRaInterfacePdu",
	&asn_OP_NULL,
	asn_DEF_MaRaInterfacePdu_tags_1,
	sizeof(asn_DEF_MaRaInterfacePdu_tags_1)
		/sizeof(asn_DEF_MaRaInterfacePdu_tags_1[0]), /* 1 */
	asn_DEF_MaRaInterfacePdu_tags_1,	/* Same as above */
	sizeof(asn_DEF_MaRaInterfacePdu_tags_1)
		/sizeof(asn_DEF_MaRaInterfacePdu_tags_1[0]), /* 1 */
	{
#if !defined(ASN_DISABLE_OER_SUPPORT)
		0,
#endif  /* !defined(ASN_DISABLE_OER_SUPPORT) */
#if !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT)
		0,
#endif  /* !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT) */
		NULL_constraint
	},
	0, 0,	/* No members */
	0	/* No specifics */
};

