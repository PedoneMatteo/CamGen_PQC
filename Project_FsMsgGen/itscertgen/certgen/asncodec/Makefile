ifneq ( $(findstring $(MAKECMDGOALS),clean regen),)
 include Makefile.am.libasncodec
endif

PROJECTROOT    = ../..
BUILDROOT      = $(PROJECTROOT)/build
CSHAREDDIR     = $(PROJECTROOT)/cshared
PROJECT        = ItsCertAsn
DEBUG          = yes
alibs          = ItsCertAsn
sources       := $(ASN_MODULE_SRCS)
includes      += .
cflags        += $(ASN_MODULE_CFLAGS)

include $(CSHAREDDIR)/common.mk

TS102941_DIR     := asn1/pki_ts102941
TS103097_DIR     := asn1/sec_ts103097
IEEE1609_2_DIR   := asn1/ieee1609.2
IEEE1609_2_1_DIR := asn1/ieee1609.2.1

#ASN1CDIR ?= ../../../../asn1c-fillabs2

ifeq (,$(ASN1C))
 ifneq (,$(ASN1CDIR))
  ASN1C := $(ASN1CDIR)/asn1c/asn1c -S $(ASN1CDIR)/skeletons
 else
  ASN1C := /usr/local/bin/asn1c
 endif
endif

ASN1C_ARGS := -no-gen-BER -no-gen-JER -no-gen-UPER -no-gen-APER -no-gen-random-fill -no-gen-example
#ASN1C_ARGS := -fcompound-names

ASN1_SRC := \
	$(TS102941_DIR)/EtsiTs102941MessagesCa.asn \
	$(TS102941_DIR)/EtsiTs102941TrustLists.asn \
	$(TS102941_DIR)/EtsiTs102941TypesAuthorization.asn \
	$(TS102941_DIR)/EtsiTs102941TypesAuthorizationValidation.asn \
	$(TS102941_DIR)/EtsiTs102941TypesCaManagement.asn \
	$(TS102941_DIR)/EtsiTs102941BaseTypes.asn \
	$(TS102941_DIR)/EtsiTs102941TypesEnrolment.asn \
	$(TS102941_DIR)/EtsiTs102941TypesLinkCertificate.asn \
    $(TS103097_DIR)/EtsiTs103097Module.asn \
    $(TS103097_DIR)/EtsiTs103097ExtensionModule.asn \
    $(IEEE1609_2_DIR)/Ieee1609Dot2.asn \
    $(IEEE1609_2_DIR)/Ieee1609Dot2BaseTypes.asn \
    $(IEEE1609_2_DIR)/Ieee1609Dot2Crl.asn \
    $(IEEE1609_2_DIR)/Ieee1609Dot2CrlBaseTypes.asn \
    $(IEEE1609_2_1_DIR)/Ieee1609Dot2Dot1AcaEeInterface.asn \
    $(IEEE1609_2_1_DIR)/Ieee1609Dot2Dot1AcaLaInterface.asn \
    $(IEEE1609_2_1_DIR)/Ieee1609Dot2Dot1AcaMaInterface.asn \
    $(IEEE1609_2_1_DIR)/Ieee1609Dot2Dot1AcaRaInterface.asn \
    $(IEEE1609_2_1_DIR)/Ieee1609Dot2Dot1Acpc.asn           \
    $(IEEE1609_2_1_DIR)/Ieee1609Dot2Dot1CamRaInterface.asn \
    $(IEEE1609_2_1_DIR)/Ieee1609Dot2Dot1CertManagement.asn \
    $(IEEE1609_2_1_DIR)/Ieee1609Dot2Dot1EcaEeInterface.asn \
    $(IEEE1609_2_1_DIR)/Ieee1609Dot2Dot1EeMaInterface.asn  \
    $(IEEE1609_2_1_DIR)/Ieee1609Dot2Dot1EeRaInterface.asn  \
    $(IEEE1609_2_1_DIR)/Ieee1609Dot2Dot1LaMaInterface.asn  \
    $(IEEE1609_2_1_DIR)/Ieee1609Dot2Dot1LaRaInterface.asn  \
    $(IEEE1609_2_1_DIR)/Ieee1609Dot2Dot1MaRaInterface.asn  \
    $(IEEE1609_2_1_DIR)/Ieee1609Dot2Dot1Protocol.asn       \

ASN1_PDU :=  \
    EtsiTs103097Data \
    EnrolmentRequestMessage EnrolmentResponseMessage \
    AuthorizationRequestMessage AuthorizationRequestMessageWithPop AuthorizationResponseMessage \
    CertificateRevocationListMessage TlmCertificateTrustListMessage RcaCertificateTrustListMessage \
    AuthorizationValidationRequestMessage AuthorizationValidationResponseMessage \
    CaCertificateRequestMessage \
    EtsiTs103097Certificate 

regen: force_regen  Makefile.am.libasncodec
force_regen:
	rm -f Makefile.am.libasncodec
Makefile.am.libasncodec: $(ASN1_SRC)
	$(ASN1C) -fcompound-names $(ASN1C_ARGS) $(addprefix -pdu=,$(ASN1_PDU)) $(patsubst %,"%",$(ASN1_SRC)) 2>&1
