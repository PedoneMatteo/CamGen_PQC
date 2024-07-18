##########################################################
#  This Makefile is to build the ASN1 library to be used
#  for external codec in Titan
#  This Makefile is expected that asn1c compiler is 
#  installed in the PATHs
##########################################################

# The name of the library
ASN_LIBRARY = libItsAsn.a
# ASN.1 files to be included in the library
ASN_FILES = AtsCAM/lib/asn1/CAM_PDU_Descriptions.asn \
            AtsCAM/lib/asn1/cdd/ITS_Container.asn \
            AtsDENM/lib/asn1/DENM_PDU_Descriptions.asn \
            AtsSecurity/lib/asn1/EtsiTs103097ExtensionModule.asn \
            AtsSecurity/lib/asn1/EtsiTs103097Module.asn \
            AtsSecurity/lib/asn1/ieee1609dot2/Ieee1609Dot2.asn \
            AtsSecurity/lib/asn1/ieee1609dot2/Ieee1609Dot2BaseTypes.asn\
            AtsIS/lib/asn1/IVIM_PDU_Descriptions.asn \
            AtsIS/lib/asn1/MAPEM_PDU_Descriptions.asn \
            AtsIS/lib/asn1/RTCMEM_PDU_Descriptions.asn \
            AtsIS/lib/asn1/SPATEM_PDU_Descriptions.asn \
            AtsIS/lib/asn1/SREM_PDU_Descriptions.asn \
            AtsIS/lib/asn1/SSEM_PDU_Descriptions.asn \
            AtsIS/lib/asn1/iso-patched/EfcDsrcApplication.asn  \
            AtsIS/lib/asn1/iso-patched/EfcDsrcGeneric.asn  \
            AtsIS/lib/asn1/iso-patched/ElectronicRegistrationIdentificationVehicleDataModule.asn \
            AtsIS/lib/asn1/iso-patched/GDD.asn \
            AtsIS/lib/asn1/iso-patched/DSRC.asn \
            AtsIS/lib/asn1/iso-patched/DSRC_noCircular.asn \
            AtsIS/lib/asn1/iso-patched/REGION.asn \
            AtsIS/lib/asn1/iso-patched/AddGrpC.asn \
            AtsIS/lib/asn1/iso-patched/AddGrpC_noCircular.asn \
            ../build/asn1/ISO_TS_14816/AVIAEINumberingAndDataStructures.asn \
            ../build/asn1/ISO_TS_17419/CITSapplMgmtIDs.asn \
            ../build/asn1/ISO_TS_19321/IVI.asn \
            AtsPki/lib/asn1/ieee_1609.2.1/Ieee1609Dot2Crl.asn \
            AtsPki/lib/asn1/ieee_1609.2.1/Ieee1609Dot2CrlBaseTypes.asn       \
            AtsPki/lib/asn1/ieee_1609.2.1/Ieee1609Dot2Dot1AcaEeInterface.asn \
            AtsPki/lib/asn1/ieee_1609.2.1/Ieee1609Dot2Dot1AcaLaInterface.asn \
            AtsPki/lib/asn1/ieee_1609.2.1/Ieee1609Dot2Dot1AcaMaInterface.asn \
            AtsPki/lib/asn1/ieee_1609.2.1/Ieee1609Dot2Dot1AcaRaInterface.asn \
            AtsPki/lib/asn1/ieee_1609.2.1/Ieee1609Dot2Dot1Acpc.asn           \
            AtsPki/lib/asn1/ieee_1609.2.1/Ieee1609Dot2Dot1CamRaInterface.asn \
            AtsPki/lib/asn1/ieee_1609.2.1/Ieee1609Dot2Dot1CertManagement.asn \
            AtsPki/lib/asn1/ieee_1609.2.1/Ieee1609Dot2Dot1EcaEeInterface.asn \
            AtsPki/lib/asn1/ieee_1609.2.1/Ieee1609Dot2Dot1EeMaInterface.asn  \
            AtsPki/lib/asn1/ieee_1609.2.1/Ieee1609Dot2Dot1EeRaInterface.asn  \
            AtsPki/lib/asn1/ieee_1609.2.1/Ieee1609Dot2Dot1LaMaInterface.asn  \
            AtsPki/lib/asn1/ieee_1609.2.1/Ieee1609Dot2Dot1LaRaInterface.asn  \
            AtsPki/lib/asn1/ieee_1609.2.1/Ieee1609Dot2Dot1MaRaInterface.asn  \
            AtsPki/lib/asn1/ieee_1609.2.1/Ieee1609Dot2Dot1Protocol.asn

#            ../build/asn1/ISO_TS_19091/ISO-TS-19091-addgrp-C-2018-patched.asn \
#            ../build/asn1/ISO_TS_19091/AddGrpC.asn \
#            ../build/asn1/ISO_TS_19091/AddGrpC_noCircular.asn \
#            ../build/asn1/ISO_TS_19091/DSRC.asn \
#            ../build/asn1/ISO_TS_19091/DSRC_REGION_noCircular.asn \
#            ../build/asn1/ISO_TS_19091/REGION.asn \

#            AtsPki/lib/asn1/EtsiTs102941BaseTypes.asn \
#            AtsPki/lib/asn1/EtsiTs102941MessagesCa.asn \
#            AtsPki/lib/asn1/EtsiTs102941TrustLists.asn \
#            AtsPki/lib/asn1/EtsiTs102941TypesAuthorization.asn \
#            AtsPki/lib/asn1/EtsiTs102941TypesAuthorizationValidation.asn \
#            AtsPki/lib/asn1/EtsiTs102941TypesCaManagement.asn \
#            AtsPki/lib/asn1/EtsiTs102941TypesEnrolment.asn \
#            AtsPki/lib/asn1/EtsiTs102941TypesLinkCertificate.asn \

# PDUs
PDU = CAM DENM MAPEM SPATEM SREM SSEM IVIM RTCMEM EtsiTs103097Data EtsiTs103097Certificate
#EtsiTs102941Data

#patches
#PATCHES = PsidGroupPermissions.c.diff

ASN1C:=asn1c
#Override ASN1C instalation path
#ASN1C_PATH = 

ifneq (, $(ASN1C_PATH))
# OS=$(shell uname)
 ifneq (Windows_NT,$(OS))
  ASN1C:=$(ASN1C_PATH)/asn1c/asn1c -S $(ASN1C_PATH)/skeletons
 else
  ASN1C:=$(ASN1C_PATH)/asn1c/asn1c.exe -S $(ASN1C_PATH)/skeletons
 endif
endif

ifeq (,$(ASN_DIR))
##########################################################
# Build path for the library
BIN_DIR := .
SCRIPT_DIR := $(dir $(lastword $(MAKEFILE_LIST)))
ASN_DIR := $(dir $(SCRIPT_DIR))../ttcn

.PHONY: FORCE

all: $(BIN_DIR)/$(ASN_LIBRARY)
clean:
	rm -rf $(BIN_DIR)

$(BIN_DIR)/$(ASN_LIBRARY): $(BIN_DIR)/Makefile
	make -C $(BIN_DIR) all

$(BIN_DIR):
	mkdir -p $@

$(BIN_DIR)/Makefile: $(BIN_DIR) $(SCRIPT_DIR)/Makefile
	echo SCRIPT_DIR=$(SCRIPT_DIR) > $@
	echo ASN_DIR=$(ASN_DIR)       >>$@
	cat $(SCRIPT_DIR)/Makefile    >>$@

##########################################################
else
##########################################################
# This part is executed within the BIN_DIR directory to
# override variables from the Makefile.am.libasncodec 
# generated by asn1c
LIB_MAKEFILE=Makefile.am.libasncodec
include $(LIB_MAKEFILE)
OBJS = ${ASN_MODULE_SRCS:.c=.o}
CONVERTER=converter
ASN_CONVERTER_SOURCES := \
	converter-example.c\
	pdu_collection.c
CONVERTER_OBJS=${ASN_CONVERTER_SOURCES:.c=.o}
CFLAGS += $(ASN_MODULE_CFLAGS) -DPDU=CAM -DASN_PDU_COLLECTION -fPIC -I.
CC = gcc -std=c99
GEN_EXAMPLE=-no-gen-example 
ifeq (yes,$(DEBUG))
  CFLAGS += -g -O0 -DASN_EMIT_DEBUG=1
endif

all: Makefile $(ASN_LIBRARY)

$(LIB_MAKEFILE): $(addprefix $(ASN_DIR)/, $(ASN_FILES))
	$(ASN1C) $(GEN_EXAMPLE) $(addprefix -pdu=,$(PDU)) -fcompound-names $^
	-for n in $(PATCHES); do git apply "$(SCRIPT_DIR)/$$n"; done

$(CONVERTER): $(ASN_LIBRARY) $(CONVERTER_OBJS) 
	$(CC) $(CFLAGS) -o $@ $(CONVERTER_OBJS) $(ASN_LIBRARY) $(LIBS)

$(ASN_LIBRARY): ${ASN_MODULE_SRCS:.c=.o}
	ar rcs $@ $^
#	$(CC) $(CFLAGS) -o $@ $^ -shared $(LDFLAGS) $(LIBS)

.SUFFIXES:
.SUFFIXES: .c .o

.c.o:
	$(CC) $(CFLAGS) -o $@ -c $<

clean:
	rm -f $(CONVERTER) $(ASN_LIBRARY)
	rm -f $(OBJS) $(CONVERTER_OBJS)

regen: clear-asn1c $(LIB_MAKEFILE)
clear-asn1c:
	rm -f $(LIB_MAKEFILE)

Makefile: $(SCRIPT_DIR)/Makefile
	echo SCRIPT_DIR=$(SCRIPT_DIR) > $@
	echo ASN_DIR=$(ASN_DIR) >>$@
	cat $<                  >>$@
##########################################################

$(ASN_DIR)/../build/asn1/ISO_TS_14816/AVIAEINumberingAndDataStructures.asn:
	mkdir -p "$(dir $@)"
	curl 'https://standards.iso.org/iso/14816/ISO14816%20ASN.1%20repository/ISO14816_AVIAEINumberingAndDataStructures.asn' | \
	sed -e 's/IssuerIdentifier/AVIAEIIssuerIdentifier/g' > "$@"

#../build/asn1/ISO_TS_14906/EfcDsrcApplication.asn:
#	mkdir -p "$(dir $@)"
#	curl -o "$@" 'https://standards.iso.org/iso/14906/ed-3/en/ISO14906(2018)EfcDsrcApplicationv6.asn'

#../build/asn1/ISO_TS_14906/EfcDsrcGeneric.asn:
#	mkdir -p "$(dir $@)
#	curl -o "$@" 'https://standards.iso.org/iso/14906/ed-3/en/ISO14906(2018)EfcDsrcGenericv7.asn'

$(ASN_DIR)/../build/asn1/ISO_TS_17419/CITSapplMgmtIDs.asn:
	mkdir -p "$(dir $@)"
	curl -o "$@" 'https://standards.iso.org/iso/ts/17419/TS%2017419%20ASN.1%20repository/TS17419_2014_CITSapplMgmtIDs.asn'

$(ASN_DIR)/../build/asn1/ISO_TS_19091/ISO-TS-19091-addgrp-C-2018-patched.asn:
	mkdir -p "$(dir $@)"
	curl 'https://standards.iso.org/iso/ts/19091/ed-2/en/ISO-TS-19091-addgrp-C-2018.asn' | \
	sed -e 's/\bHeadingConfidence\b/HeadingConfidenceDSRC/g' \
        -e 's/\bSpeedConfidence\b/SpeedConfidenceDSRC/g' \
        -e 's/\bHeading\b/HeadingDSRC/g' > "$@"

$(ASN_DIR)/../build/asn1/ISO_TS_17419/CITSdataDictionary1.asn:
	mkdir -p "$(dir $@)"
	curl -o "$@" 'https://standards.iso.org/iso/17419/ed-1/en/17419.1.asn'

$(ASN_DIR)/../build/asn1/ISO_TS_17419/CITSdataDictionary2.asn:
	mkdir -p "$(dir $@)"
	curl -o "$@" 'https://standards.iso.org/iso/17419/ed-1/en/17419.2.asn'

#../build/asn1/ISO_TS_19091/AddGrpC.asn:
#../build/asn1/ISO_TS_19091/AddGrpC_noCircular.asn:
#../build/asn1/ISO_TS_19091/DSRC.asn:
#../build/asn1/ISO_TS_19091/DSRC_REGION_noCircular.asn
#../build/asn1/ISO_TS_19091/REGION.asn

$(ASN_DIR)/../build/asn1/ISO_TS_19321/IVI.asn:
	mkdir -p "$(dir $@)"
	curl 'https://standards.iso.org/iso/ts/19321/ed-2/en/ISO19321IVIv2.asn' | \
	sed -e 's/\bCITSdataDictionary1\b/CITSapplMgmtIDs/g' \
	    -e 's/,\s*\.\.\.\s*,\s*[0-9]\+/,.../g' \
	> "$@"

#$(ASN_DIR)/../build/asn1/ISO_TS_24534-3/ElectronicRegistrationIdentificationVehicleDataModule.asn:
#	mkdir -p "$(dir $@)"
#	curl -o "$@" 'https://standards.iso.org/iso/24534/-3/ISO%2024534-3%20ASN.1%20repository/ISO24534-3_ElectronicRegistrationIdentificationVehicleDataModule_ForBallot.asn'

endif
