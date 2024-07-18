sources := \
           LibItsGeoNetworking_Encdec.cc \
           LibItsIpv6OverGeoNetworking_Encdec.cc \
           LibItsSecurity_Encdec.cc \


# AtsGeoNetworking and AtsPki do not require LibItsBtp_Encdec.cc

ifeq (AtsBTP, $(ATS))
sources += LibItsBtp_Encdec.cc
endif

ifeq (AtsCAM, $(ATS))
sources += LibItsCam_Encdec.cc \
           LibItsBtp_Encdec.cc
endif

ifeq (AtsDENM, $(ATS))
sources += LibItsDenm_Encdec.cc \
           LibItsBtp_Encdec.cc
endif

ifeq (AtsRSUsSimulator, $(ATS))
sources += \
           LibItsBtp_Encdec.cc  \
           LibItsCam_Encdec.cc  \
           LibItsDenm_Encdec.cc  \
           LibItsHttp_Encdec.cc \
           LibItsMapemSpatem_Encdec.cc \
           LibItsSremSsem_Encdec.cc \
           LibItsRtcmem_Encdec.cc \
           LibItsIvim_Encdec.cc \
           LibItsPki_Encdec.cc

endif

ifeq (AtsSecurity, $(ATS))
sources += \
           LibItsBtp_Encdec.cc \
           LibItsCam_Encdec.cc  \
           LibItsDenm_Encdec.cc
endif

ifeq (AtsIS, $(ATS))
sources += \
           LibItsBtp_Encdec.cc \
           LibItsIvim_Encdec.cc \
           LibItsSremSsem_Encdec.cc \
           LibItsMapemSpatem_Encdec.cc \
           LibItsRtcmem_Encdec.cc
endif

ifeq (AtsPki, $(ATS))
sources += \
           LibItsBtp_Encdec.cc \
           LibItsPki_Encdec.cc \
           LibItsCam_Encdec.cc \
           LibItsHttp_Encdec.cc
endif
