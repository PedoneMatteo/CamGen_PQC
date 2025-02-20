/*
 * Generated by asn1c-0.9.29-DF (http://lionet.info/asn1c)
 * From ASN.1 module "Ieee1609Dot2Dot1AcaRaInterface"
 * 	found in "asn1/ieee1609.2.1/Ieee1609Dot2Dot1AcaRaInterface.asn"
 * 	`asn1c -fcompound-names -no-gen-BER -no-gen-JER -no-gen-UPER -no-gen-APER -no-gen-random-fill -no-gen-example -pdu=EtsiTs103097Data -pdu=EnrolmentRequestMessage -pdu=EnrolmentResponseMessage -pdu=AuthorizationRequestMessage -pdu=AuthorizationRequestMessageWithPop -pdu=AuthorizationResponseMessage -pdu=CertificateRevocationListMessage -pdu=TlmCertificateTrustListMessage -pdu=RcaCertificateTrustListMessage -pdu=AuthorizationValidationRequestMessage -pdu=AuthorizationValidationResponseMessage -pdu=CaCertificateRequestMessage -pdu=EtsiTs103097Certificate`
 */

#include "RaAcaCertRequestFlags.h"

int
RaAcaCertRequestFlags_constraint(const asn_TYPE_descriptor_t *td, const void *sptr,
			asn_app_constraint_failed_f *ctfailcb, void *app_key) {
	const BIT_STRING_t *st = (const BIT_STRING_t *)sptr;
	size_t size;
	
	if(!sptr) {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: value not given (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
	
	if(st->size > 0) {
		/* Size in bits */
		size = 8 * st->size - (st->bits_unused & 0x07);
	} else {
		size = 0;
	}
	
	if((size == 8)) {
		/* Constraint check succeeded */
		return 0;
	} else {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: constraint failed (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
}

/*
 * This type is implemented using BIT_STRING,
 * so here we adjust the DEF accordingly.
 */
#if !defined(ASN_DISABLE_OER_SUPPORT)
static asn_oer_constraints_t asn_OER_type_RaAcaCertRequestFlags_constr_1 CC_NOTUSED = {
	{ 0, 0 },
	8	/* (SIZE(8..8)) */};
#endif  /* !defined(ASN_DISABLE_OER_SUPPORT) */
static const ber_tlv_tag_t asn_DEF_RaAcaCertRequestFlags_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (3 << 2))
};
asn_TYPE_descriptor_t asn_DEF_RaAcaCertRequestFlags = {
	"RaAcaCertRequestFlags",
	"RaAcaCertRequestFlags",
	&asn_OP_BIT_STRING,
	asn_DEF_RaAcaCertRequestFlags_tags_1,
	sizeof(asn_DEF_RaAcaCertRequestFlags_tags_1)
		/sizeof(asn_DEF_RaAcaCertRequestFlags_tags_1[0]), /* 1 */
	asn_DEF_RaAcaCertRequestFlags_tags_1,	/* Same as above */
	sizeof(asn_DEF_RaAcaCertRequestFlags_tags_1)
		/sizeof(asn_DEF_RaAcaCertRequestFlags_tags_1[0]), /* 1 */
	{
#if !defined(ASN_DISABLE_OER_SUPPORT)
		&asn_OER_type_RaAcaCertRequestFlags_constr_1,
#endif  /* !defined(ASN_DISABLE_OER_SUPPORT) */
#if !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT)
		0,
#endif  /* !defined(ASN_DISABLE_UPER_SUPPORT) || !defined(ASN_DISABLE_APER_SUPPORT) */
		RaAcaCertRequestFlags_constraint
	},
	0, 0,	/* Defined elsewhere */
	&asn_SPC_BIT_STRING_specs	/* Additional specs */
};

