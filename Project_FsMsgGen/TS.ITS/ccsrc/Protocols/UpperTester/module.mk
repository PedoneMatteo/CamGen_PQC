#to be added globally
sources += uppertester_geonetworking_codec.cc uppertester_geonetworking_layer.cc

ifeq (AtsCAM, $(ATS))
sources += uppertester_cam_codec.cc uppertester_cam_layer.cc
endif
#ifeq (AtsGeoNetworking, $(ATS))
#sources += uppertester_geonetworking_codec.cc uppertester_geonetworking_layer.cc
#endif

ifeq (AtsDENM, $(ATS))
sources += uppertester_denm_codec.cc uppertester_denm_layer.cc
endif

ifeq (AtsIS, $(ATS))
sources += uppertester_ivim_codec.cc uppertester_ivim_layer.cc \
           uppertester_mapem_spatem_codec.cc uppertester_mapem_spatem_layer.cc \
           UpperTesterSremSsemCodec.cc UpperTesterSremSsemLayer.cc \
           UpperTesterRtcmemCodec.cc UpperTesterRtcmemLayer.cc
endif

ifeq (AtsPki, $(ATS))
sources += uppertester_pki_codec.cc uppertester_pki_layer.cc uppertester_cam_codec.cc
endif

ifeq (AtsSecurity, $(ATS))
sources += uppertester_cam_codec.cc uppertester_cam_layer.cc \
           uppertester_denm_codec.cc uppertester_denm_layer.cc \

endif

ifeq (AtsRSUsSimulator, $(ATS))
sources += uppertester_cam_codec.cc uppertester_cam_layer.cc \
           uppertester_denm_codec.cc uppertester_denm_layer.cc \
           uppertester_ivim_codec.cc uppertester_ivim_layer.cc \
           UpperTesterSremSsemCodec.cc UpperTesterSremSsemLayer.cc \
           uppertester_mapem_spatem_codec.cc uppertester_mapem_spatem_layer.cc \
           UpperTesterRtcmemCodec.cc UpperTesterRtcmemLayer.cc \
           uppertester_pki_codec.cc uppertester_pki_layer.cc \

endif


includes := .

modules += ../UDP
