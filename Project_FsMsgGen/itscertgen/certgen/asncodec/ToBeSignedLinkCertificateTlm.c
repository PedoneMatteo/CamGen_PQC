/*
 * Generated by asn1c-0.9.29-DF (http://lionet.info/asn1c)
 * From ASN.1 module "EtsiTs102941TypesLinkCertificate"
 * 	found in "asn1/pki_ts102941/EtsiTs102941TypesLinkCertificate.asn"
 * 	`asn1c -S ../../../../asn1c-fillabs2/skeletons -fcompound-names -no-gen-BER -no-gen-JER -no-gen-UPER -no-gen-APER -no-gen-random-fill -no-gen-example -pdu=EtsiTs103097Data -pdu=EnrolmentRequestMessage -pdu=EnrolmentResponseMessage -pdu=AuthorizationRequestMessage -pdu=AuthorizationRequestMessageWithPop -pdu=AuthorizationResponseMessage -pdu=CertificateRevocationListMessage -pdu=TlmCertificateTrustListMessage -pdu=RcaCertificateTrustListMessage -pdu=AuthorizationValidationRequestMessage -pdu=AuthorizationValidationResponseMessage -pdu=CaCertificateRequestMessage -pdu=EtsiTs103097Certificate`
 */

#include "ToBeSignedLinkCertificateTlm.h"

/*
 * This type is implemented using ToBeSignedLinkCertificate,
 * so here we adjust the DEF accordingly.
 */
static const ber_tlv_tag_t asn_DEF_ToBeSignedLinkCertificateTlm_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
asn_TYPE_descriptor_t asn_DEF_ToBeSignedLinkCertificateTlm = {
	"ToBeSignedLinkCertificateTlm",
	"ToBeSignedLinkCertificateTlm",
	&asn_OP_SEQUENCE,
	asn_DEF_ToBeSignedLinkCertificateTlm_tags_1,
	sizeof(asn_DEF_ToBeSignedLinkCertificateTlm_tags_1)
		/sizeof(asn_DEF_ToBeSignedLinkCertificateTlm_tags_1[0]), /* 1 */
	asn_DEF_ToBeSignedLinkCertificateTlm_tags_1,	/* Same as above */
	sizeof(asn_DEF_ToBeSignedLinkCertificateTlm_tags_1)
		/sizeof(asn_DEF_ToBeSignedLinkCertificateTlm_tags_1[0]), /* 1 */
	{
#if !defined(ASN_DISABLE_OER_SUPPORT)
		0,
#endif  /* !defined(ASN_DISABLE_OER_SUPPORT) */
#if !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT)
		0,
#endif  /* !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT) */
		SEQUENCE_constraint
	},
	asn_MBR_ToBeSignedLinkCertificate_1,
	2,	/* Elements count */
	&asn_SPC_ToBeSignedLinkCertificate_specs_1	/* Additional specs */
};

