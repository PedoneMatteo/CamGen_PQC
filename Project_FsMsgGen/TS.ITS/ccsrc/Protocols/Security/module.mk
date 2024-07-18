sources := certificates_loader.cc \
           etsi_ts103097_certificate_codec.cc \
           etsi_ts103097_data_codec.cc \
           etsi_ts103097_tobesigned_certificate_codec.cc \
           etsi_ts103097_tobesigned_data_codec.cc \
           hmac.cc \
           ieee_1609dot2_base_types_public_encryption_key.cc \
           ieee_1609dot2_base_types_public_verification_key.cc \
           security_cache.cc \
           security_db.cc \
           security_db_record.cc \
           security_ecc.cc \
           security_services.cc \
           sha256.cc \
           sha384.cc \

ifeq (AtsPki, $(ATS))
sources += http_etsi_ieee1609dot2_codec.cc \
           etsi_ts102941_base_types_public_keys.cc \
           etsi_ts102941_data_codec.cc \
           etsi_ts102941_data_content_codec.cc \
           etsi_ts102941_types_authorization_inner_request.cc \
           etsi_ts102941_types_authorization_inner_response.cc \
           etsi_ts102941_types_authorization_shared_at_request.cc \
           etsi_ts102941_types_authorization_validation_request.cc \
           etsi_ts102941_types_authorization_validation_response.cc \
           etsi_ts102941_types_enrolment_inner_request.cc \
           etsi_ts102941_types_enrolment_inner_response.cc \

endif

ifeq (AtsRSUsSimulator, $(ATS))
sources += http_etsi_ieee1609dot2_codec.cc \
           etsi_ts102941_base_types_public_keys.cc \
           etsi_ts102941_data_codec.cc \
           etsi_ts102941_data_content_codec.cc \
           etsi_ts102941_types_authorization_inner_request.cc \
           etsi_ts102941_types_authorization_inner_response.cc \
           etsi_ts102941_types_authorization_shared_at_request.cc \
           etsi_ts102941_types_authorization_validation_request.cc \
           etsi_ts102941_types_authorization_validation_response.cc \
           etsi_ts102941_types_enrolment_inner_request.cc \
           etsi_ts102941_types_enrolment_inner_response.cc \

endif

includes := .
