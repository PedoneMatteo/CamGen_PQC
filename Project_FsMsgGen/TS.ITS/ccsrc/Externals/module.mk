sources := LibItsCommon_externals.cc \
           LibItsGeoNetworking_externals.cc \
           LibItsIpv6OverGeoNetworking_externals.cc \

includes := ../geospacial


ifeq (AtsCAM, $(ATS))
sources +=  LibItsSecurity_externals.cc
endif

ifeq (AtsDENM, $(ATS))
sources +=  LibItsSecurity_externals.cc
endif

ifeq (AtsIS, $(ATS))
sources +=  LibItsSecurity_externals.cc
endif

ifeq (AtsSecurity, $(ATS))
sources +=  LibItsSecurity_externals.cc
endif

ifeq (AtsPki, $(ATS))
sources +=  LibItsPki_externals.cc LibItsSecurity_externals.cc
endif

ifeq (AtsGeoNetworking, $(ATS))
sources +=  LibItsSecurity_externals.cc
endif

ifeq (AtsIPv6OverGeoNetworking, $(ATS))
sources +=  LibItsSecurity_externals.cc
endif

ifeq (AtsBTP, $(ATS))
sources +=  LibItsSecurity_externals.cc
endif

ifeq (AtsRSUsSimulator, $(ATS))
sources += LibItsPki_externals.cc LibItsSecurity_externals.cc AtsRSUsSimulator_externals.cc
endif

#           LibItsDcc_externals.cc \
