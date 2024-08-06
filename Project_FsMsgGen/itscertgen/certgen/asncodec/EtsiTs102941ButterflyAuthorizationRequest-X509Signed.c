/*
 * Generated by asn1c-0.9.29-DF (http://lionet.info/asn1c)
 * From ASN.1 module "EtsiTs102941TypesAuthorization"
 * 	found in "asn1/pki_ts102941/EtsiTs102941TypesAuthorization.asn"
 * 	`asn1c -fcompound-names -no-gen-BER -no-gen-JER -no-gen-UPER -no-gen-APER -no-gen-random-fill -no-gen-example -pdu=EtsiTs103097Data -pdu=EnrolmentRequestMessage -pdu=EnrolmentResponseMessage -pdu=AuthorizationRequestMessage -pdu=AuthorizationRequestMessageWithPop -pdu=AuthorizationResponseMessage -pdu=CertificateRevocationListMessage -pdu=TlmCertificateTrustListMessage -pdu=RcaCertificateTrustListMessage -pdu=AuthorizationValidationRequestMessage -pdu=AuthorizationValidationResponseMessage -pdu=CaCertificateRequestMessage -pdu=EtsiTs103097Certificate`
 */

#include "EtsiTs102941ButterflyAuthorizationRequest-X509Signed.h"

int
EtsiTs102941ButterflyAuthorizationRequest_X509Signed_constraint(const asn_TYPE_descriptor_t *td, const void *sptr,
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
 * This type is implemented using Ieee1609Dot2Data_SignedX509AuthenticatedCertRequest_335P0,
 * so here we adjust the DEF accordingly.
 */
#if !defined(ASN_DISABLE_OER_SUPPORT)
static asn_oer_constraints_t asn_OER_type_EtsiTs102941ButterflyAuthorizationRequest_X509Signed_constr_1 CC_NOTUSED = {
	{ 0, 0 },
	-1};
#endif  /* !defined(ASN_DISABLE_OER_SUPPORT) */
static const ber_tlv_tag_t asn_DEF_EtsiTs102941ButterflyAuthorizationRequest_X509Signed_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
asn_TYPE_descriptor_t asn_DEF_EtsiTs102941ButterflyAuthorizationRequest_X509Signed = {
	"EtsiTs102941ButterflyAuthorizationRequest-X509Signed",
	"EtsiTs102941ButterflyAuthorizationRequest-X509Signed",
	&asn_OP_SEQUENCE,
	asn_DEF_EtsiTs102941ButterflyAuthorizationRequest_X509Signed_tags_1,
	sizeof(asn_DEF_EtsiTs102941ButterflyAuthorizationRequest_X509Signed_tags_1)
		/sizeof(asn_DEF_EtsiTs102941ButterflyAuthorizationRequest_X509Signed_tags_1[0]), /* 1 */
	asn_DEF_EtsiTs102941ButterflyAuthorizationRequest_X509Signed_tags_1,	/* Same as above */
	sizeof(asn_DEF_EtsiTs102941ButterflyAuthorizationRequest_X509Signed_tags_1)
		/sizeof(asn_DEF_EtsiTs102941ButterflyAuthorizationRequest_X509Signed_tags_1[0]), /* 1 */
	{
#if !defined(ASN_DISABLE_OER_SUPPORT)
		&asn_OER_type_EtsiTs102941ButterflyAuthorizationRequest_X509Signed_constr_1,
#endif  /* !defined(ASN_DISABLE_OER_SUPPORT) */
#if !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT)
		0,
#endif  /* !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT) */
		EtsiTs102941ButterflyAuthorizationRequest_X509Signed_constraint
	},
	asn_MBR_Ieee1609Dot2Data_1,
	2,	/* Elements count */
	&asn_SPC_Ieee1609Dot2Data_specs_1	/* Additional specs */
};

