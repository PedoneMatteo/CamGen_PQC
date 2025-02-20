/*
 * Generated by asn1c-0.9.29-DF (http://lionet.info/asn1c)
 * From ASN.1 module "EtsiTs102941TrustLists"
 * 	found in "asn1/pki_ts102941/EtsiTs102941TrustLists.asn"
 * 	`asn1c -fcompound-names -no-gen-BER -no-gen-JER -no-gen-UPER -no-gen-APER -no-gen-random-fill -no-gen-example -pdu=EtsiTs103097Data -pdu=EnrolmentRequestMessage -pdu=EnrolmentResponseMessage -pdu=AuthorizationRequestMessage -pdu=AuthorizationRequestMessageWithPop -pdu=AuthorizationResponseMessage -pdu=CertificateRevocationListMessage -pdu=TlmCertificateTrustListMessage -pdu=RcaCertificateTrustListMessage -pdu=AuthorizationValidationRequestMessage -pdu=AuthorizationValidationResponseMessage -pdu=CaCertificateRequestMessage -pdu=EtsiTs103097Certificate`
 */

#include "Url.h"

/*
 * This type is implemented using IA5String,
 * so here we adjust the DEF accordingly.
 */
static const ber_tlv_tag_t asn_DEF_Url_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (22 << 2))
};
asn_TYPE_descriptor_t asn_DEF_Url = {
	"Url",
	"Url",
	&asn_OP_IA5String,
	asn_DEF_Url_tags_1,
	sizeof(asn_DEF_Url_tags_1)
		/sizeof(asn_DEF_Url_tags_1[0]), /* 1 */
	asn_DEF_Url_tags_1,	/* Same as above */
	sizeof(asn_DEF_Url_tags_1)
		/sizeof(asn_DEF_Url_tags_1[0]), /* 1 */
	{
#if !defined(ASN_DISABLE_OER_SUPPORT)
		0,
#endif  /* !defined(ASN_DISABLE_OER_SUPPORT) */
#if !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT)
		0,
#endif  /* !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT) */
		IA5String_constraint
	},
	0, 0,	/* No members */
	0	/* No specifics */
};

