#include "LibItsPki_EncdecDeclarations.hh"

#include "etsi_ts102941_base_types_public_keys.hh"
#include "etsi_ts102941_data_codec.hh"
#include "etsi_ts102941_data_content_codec.hh"
#include "etsi_ts102941_types_authorization_inner_request.hh"
#include "etsi_ts102941_types_authorization_inner_response.hh"
#include "etsi_ts102941_types_authorization_shared_at_request.hh"
#include "etsi_ts102941_types_authorization_validation_request.hh"
#include "etsi_ts102941_types_authorization_validation_response.hh"
#include "etsi_ts102941_types_enrolment_inner_request.hh"
#include "etsi_ts102941_types_enrolment_inner_response.hh"
#include "ieee_1609dot2_base_types_public_encryption_key.hh"
#include "ieee_1609dot2_base_types_public_verification_key.hh"

#include "etsi_ts102941_trust_lists_ctl_format.hh"

#include "loggers.hh"

//#include "cam_pdu_codec.hh"
//#include "codec.hh"

namespace LibItsPki__EncdecDeclarations {

  /*BITSTRING fx__enc__CAM(const CAM__PDU__Descriptions::CAM &p_cam) {
    cam_pdu_codec asn_codec;
    BITSTRING b;
    int rc = asn_codec.encode(p_cam, b);
    if (rc) {
      return b;
    }
    return int2bit(0, 1);
    }*/

  BITSTRING fx__enc__EtsiTs102941Data(const EtsiTs102941MessagesCa::EtsiTs102941Data &p_etsi_ts_102941_data) {
    loggers::get_instance().log_msg(">>> fx__enc__EtsiTs102941Data: ", p_etsi_ts_102941_data);

    etsi_ts102941_data_codec codec;
    OCTETSTRING              os;
    if (codec.encode(p_etsi_ts_102941_data, os) == -1) {
      loggers::get_instance().warning("fx__enc__EtsiTs102941Data: -1 result code was returned");
      return int2bit(0, 1);
    }

    return oct2bit(os);
  }

  INTEGER fx__dec__EtsiTs102941Data(BITSTRING &b, EtsiTs102941MessagesCa::EtsiTs102941Data &p_etsi_ts_102941_data) {
    loggers::get_instance().log_msg(">>> fx__dec__EtsiTs102941Data: ", b);

    etsi_ts102941_data_codec codec;
    OCTETSTRING              is = bit2oct(b);
    if (codec.decode(is, p_etsi_ts_102941_data) == -1) {
      loggers::get_instance().warning("fx__dec__EtsiTs102941Data: -1 result code was returned");
      return -1;
    }

    return 0;
  }

  BITSTRING fx__enc__EtsiTs102941DataContent(const EtsiTs102941MessagesCa::EtsiTs102941DataContent &p_etsi_ts_102941_data_content) {
    loggers::get_instance().log_msg(">>> fx__enc__EtsiTs102941DataContent: ", p_etsi_ts_102941_data_content);

    etsi_ts102941_data_content_codec codec;
    OCTETSTRING                      os;
    if (codec.encode(p_etsi_ts_102941_data_content, os) == -1) {
      loggers::get_instance().warning("fx__enc__EtsiTs102941DataContent: -1 result code was returned");
      return int2bit(0, 1);
    }

    return oct2bit(os);
  }

  INTEGER fx__dec__EtsiTs102941DataContent(BITSTRING &b, EtsiTs102941MessagesCa::EtsiTs102941DataContent &p_etsi_ts_102941_data_content) {
    loggers::get_instance().log_msg(">>> fx__dec__EtsiTs102941DataContent: ", b);

    etsi_ts102941_data_content_codec codec;
    OCTETSTRING                      is = bit2oct(b);
    if (codec.decode(is, p_etsi_ts_102941_data_content) == -1) {
      loggers::get_instance().warning("fx__dec__EtsiTs102941DataContent: -1 result code was returned");
      return -1;
    }

    return 0;
  }

  BITSTRING fx__enc__InnerEcRequest(const EtsiTs102941TypesEnrolment::InnerEcRequest &p_inner_request) {
    loggers::get_instance().log_msg(">>> fx_enc_InnerEcRequest: ", p_inner_request);

    etsi_ts102941_types_enrolment_inner_request codec;
    OCTETSTRING                                 os;
    if (codec.encode(p_inner_request, os) == -1) {
      loggers::get_instance().warning("fx_enc_InnerEcRequest: -1 result code was returned");
      return int2bit(0, 1);
    }

    return oct2bit(os);
  }

  INTEGER fx__dec__InnerEcRequest(BITSTRING &b, EtsiTs102941TypesEnrolment::InnerEcRequest &p_inner_request) {
    loggers::get_instance().log_msg(">>> fx__dec__InnerEcRequest: ", b);

    etsi_ts102941_types_enrolment_inner_request codec;
    OCTETSTRING                                 is = bit2oct(b);
    if (codec.decode(is, p_inner_request) == -1) {
      loggers::get_instance().warning("fx__dec__InnerEcRequest: -1 result code was returned");
      return -1;
    }

    loggers::get_instance().log_msg("<<< fx__dec__InnerEcRequest: ", p_inner_request);
    return 0;
  }

  BITSTRING fx__enc__InnerEcResponse(const EtsiTs102941TypesEnrolment::InnerEcResponse &p_inner_response) {
    loggers::get_instance().log_msg(">>> fx__enc__InnerEcResponse: ", p_inner_response);

    etsi_ts102941_types_enrolment_inner_response codec;
    OCTETSTRING                                  os;
    if (codec.encode(p_inner_response, os) == -1) {
      loggers::get_instance().warning("fx__enc__InnerEcResponse: -1 result code was returned");
      return int2bit(0, 1);
    }

    return oct2bit(os);
  }

  INTEGER fx__dec__InnerEcResponse(BITSTRING &b, EtsiTs102941TypesEnrolment::InnerEcResponse &p_inner_response) {
    loggers::get_instance().log_msg(">>> fx__dec__InnerEcResponse: ", b);

    etsi_ts102941_types_enrolment_inner_response codec;
    OCTETSTRING                                  is = bit2oct(b);
    if (codec.decode(is, p_inner_response) == -1) {
      loggers::get_instance().warning("fx__dec__InnerEcResponse: -1 result code was returned");
      return -1;
    }

    loggers::get_instance().log_msg("<<< fx__dec__InnerEcResponse: ", p_inner_response);
    return 0;
  }

  BITSTRING fx__enc__InnerAtRequest(const EtsiTs102941TypesAuthorization::InnerAtRequest &p_inner_request) {
    loggers::get_instance().log_msg(">>> fx_enc_InnerAtRequest: ", p_inner_request);

    etsi_ts102941_types_authorization_inner_request codec;
    OCTETSTRING                                     os;
    if (codec.encode(p_inner_request, os) == -1) {
      loggers::get_instance().warning("fx_enc_InnerAtRequest: -1 result code was returned");
      return int2bit(0, 1);
    }

    return oct2bit(os);
  }

  INTEGER fx__dec__InnerAtRequest(BITSTRING &b, EtsiTs102941TypesAuthorization::InnerAtRequest &p_inner_request) {
    loggers::get_instance().log_msg(">>> fx__dec__InnerAtRequest: ", b);

    etsi_ts102941_types_authorization_inner_request codec;
    OCTETSTRING                                     is = bit2oct(b);
    if (codec.decode(is, p_inner_request) == -1) {
      loggers::get_instance().warning("fx__dec__InnerAtRequest: -1 result code was returned");
      return -1;
    }

    loggers::get_instance().log_msg("<<< fx__dec__InnerAtRequest: ", p_inner_request);
    return 0;
  }

  BITSTRING fx__enc__InnerAtResponse(const EtsiTs102941TypesAuthorization::InnerAtResponse &p_inner_response) {
    loggers::get_instance().log_msg(">>> fx__enc__InnerAtResponse: ", p_inner_response);

    etsi_ts102941_types_authorization_inner_response codec;
    OCTETSTRING                                      os;
    if (codec.encode(p_inner_response, os) == -1) {
      loggers::get_instance().warning("fx__enc__InnerAtResponse: -1 result code was returned");
      return int2bit(0, 1);
    }

    return oct2bit(os);
  }

  INTEGER fx__dec__InnerAtResponse(BITSTRING &b, EtsiTs102941TypesAuthorization::InnerAtResponse &p_inner_response) {
    loggers::get_instance().log_msg(">>> fx__dec__InnerAtResponse: ", b);

    etsi_ts102941_types_authorization_inner_response codec;
    OCTETSTRING                                      is = bit2oct(b);
    if (codec.decode(is, p_inner_response) == -1) {
      loggers::get_instance().warning("fx__dec__InnerAtResponse: -1 result code was returned");
      return -1;
    }

    loggers::get_instance().log_msg("<<< fx__dec__InnerAtResponse: ", p_inner_response);
    return 0;
  }

  BITSTRING fx__enc__SharedAtRequest(EtsiTs102941TypesAuthorization::SharedAtRequest const &p_shared_at) {
    loggers::get_instance().log_msg(">>> fx__enc__SharedAtRequest: ", p_shared_at);

    etsi_ts102941_types_authorization_shared_at_request codec;
    OCTETSTRING                                         os;
    if (codec.encode(p_shared_at, os) == -1) {
      loggers::get_instance().warning("fx__enc__SharedAtRequest: -1 result code was returned");
      return int2bit(0, 1);
    }

    return oct2bit(os);
  }

  INTEGER fx__dec__SharedAtRequest(BITSTRING &b, EtsiTs102941TypesAuthorization::SharedAtRequest &p_shared_at) {
    loggers::get_instance().log_msg(">>> fx__dec__SharedAtRequest: ", b);

    etsi_ts102941_types_authorization_shared_at_request codec;
    OCTETSTRING                                         is = bit2oct(b);
    if (codec.decode(is, p_shared_at) == -1) {
      loggers::get_instance().warning("fx__dec__SharedAtRequest: -1 result code was returned");
      return -1;
    }

    loggers::get_instance().log_msg("<<< fx__dec__SharedAtRequest: ", p_shared_at);
    return 0;
  }

  BITSTRING fx__enc__AuthorizationValidationRequest(EtsiTs102941TypesAuthorizationValidation::AuthorizationValidationRequest const &p_shared_at) {
    loggers::get_instance().log_msg(">>> fx__enc__AuthorizationValidationRequest: ", p_shared_at);

    etsi_ts102941_types_authorization_validation_request codec;
    OCTETSTRING                                          os;
    if (codec.encode(p_shared_at, os) == -1) {
      loggers::get_instance().warning("fx__enc__AuthorizationValidationRequest: -1 result code was returned");
      return int2bit(0, 1);
    }

    return oct2bit(os);
  }

  INTEGER fx__dec__AuthorizationValidationRequest(BITSTRING &b, EtsiTs102941TypesAuthorizationValidation::AuthorizationValidationRequest &p_shared_at) {
    loggers::get_instance().log_msg(">>> fx__dec__AuthorizationValidationRequest: ", b);

    etsi_ts102941_types_authorization_validation_request codec;
    OCTETSTRING                                          is = bit2oct(b);
    if (codec.decode(is, p_shared_at) == -1) {
      loggers::get_instance().warning("fx__dec__AuthorizationValidationRequest: -1 result code was returned");
      return -1;
    }

    loggers::get_instance().log_msg("<<< fx__dec__AuthorizationValidationRequest: ", p_shared_at);
    return 0;
  }

  BITSTRING fx__enc__AuthorizationValidationResponse(EtsiTs102941TypesAuthorizationValidation::AuthorizationValidationResponse const &p_shared_at) {
    loggers::get_instance().log_msg(">>> fx__enc__AuthorizationValidationResponse: ", p_shared_at);

    etsi_ts102941_types_authorization_validation_response codec;
    OCTETSTRING                                           os;
    if (codec.encode(p_shared_at, os) == -1) {
      loggers::get_instance().warning("fx__enc__AuthorizationValidationResponse: -1 result code was returned");
      return int2bit(0, 1);
    }

    return oct2bit(os);
  }

  INTEGER fx__dec__AuthorizationValidationResponse(BITSTRING &b, EtsiTs102941TypesAuthorizationValidation::AuthorizationValidationResponse &p_shared_at) {
    loggers::get_instance().log_msg(">>> fx__dec__AuthorizationValidationResponse: ", b);

    etsi_ts102941_types_authorization_validation_response codec;
    OCTETSTRING                                           is = bit2oct(b);
    if (codec.decode(is, p_shared_at) == -1) {
      loggers::get_instance().warning("fx__dec__AuthorizationValidationResponse: -1 result code was returned");
      return -1;
    }

    loggers::get_instance().log_msg("<<< fx__dec__AuthorizationValidationResponse: ", p_shared_at);
    return 0;
  }

  BITSTRING fx__enc__PublicKeys(EtsiTs102941BaseTypes::PublicKeys const &p_public_keys) {
    loggers::get_instance().log_msg(">>> fx__enc__PublicKeys: ", p_public_keys);

    etsi_ts102941_base_types_public_keys codec;
    OCTETSTRING                          os;
    if (codec.encode(p_public_keys, os) == -1) {
      loggers::get_instance().warning("fx__enc__PublicKeys: -1 result code was returned");
      return int2bit(0, 1);
    }

    return oct2bit(os);
  }

  INTEGER fx__dec__PublicKeys(BITSTRING &b, EtsiTs102941BaseTypes::PublicKeys &p_public_keys) {
    loggers::get_instance().log_msg(">>> fx__dec__PublicKeys: ", b);

    etsi_ts102941_base_types_public_keys codec;
    OCTETSTRING                          is = bit2oct(b);
    if (codec.decode(is, p_public_keys) == -1) {
      loggers::get_instance().warning("fx__dec__PublicKeys: -1 result code was returned");
      return -1;
    }

    loggers::get_instance().log_msg("<<< fx__dec__PublicKeys: ", p_public_keys);
    return 0;
  }

  BITSTRING fx__enc__PublicVerificationKey(Ieee1609Dot2BaseTypes::PublicVerificationKey const &p_public_verification_key) {
    loggers::get_instance().log_msg(">>> fx__enc__PublicVerificationKey: ", p_public_verification_key);

    ieee_1609dot2_base_types_public_verification_key codec;
    OCTETSTRING                                      os;
    if (codec.encode(p_public_verification_key, os) == -1) {
      loggers::get_instance().warning("fx__enc__PublicVerificationKey: -1 result code was returned");
      return int2bit(0, 1);
    }

    return oct2bit(os);
  }

  INTEGER fx__dec__PublicVerificationKey(BITSTRING &b, Ieee1609Dot2BaseTypes::PublicVerificationKey &p_public_verification_key) {
    loggers::get_instance().log_msg(">>> fx__dec__PublicVerificationKey: ", b);

    ieee_1609dot2_base_types_public_verification_key codec;
    OCTETSTRING                                      is = bit2oct(b);
    if (codec.decode(is, p_public_verification_key) == -1) {
      loggers::get_instance().warning("fx__dec__PublicVerificationKey: -1 result code was returned");
      return -1;
    }

    loggers::get_instance().log_msg("<<< fx__dec__PublicVerificationKey: ", p_public_verification_key);
    return 0;
  }

  BITSTRING fx__enc__PublicEncryptionKey(Ieee1609Dot2BaseTypes::PublicEncryptionKey const &p_public_encryption_key) {
    loggers::get_instance().log_msg(">>> fx__enc__PublicEncryptionKey: ", p_public_encryption_key);

    ieee_1609dot2_base_types_public_encryption_key codec;
    OCTETSTRING                                    os;
    if (codec.encode(p_public_encryption_key, os) == -1) {
      loggers::get_instance().warning("fx__enc__PublicEncryptionKey: -1 result code was returned");
      return int2bit(0, 1);
    }

    return oct2bit(os);
  }

  INTEGER fx__dec__PublicEncryptionKey(BITSTRING &b, Ieee1609Dot2BaseTypes::PublicEncryptionKey &p_public_encryption_key) {
    loggers::get_instance().log_msg(">>> fx__dec__PublicEncryptionKey: ", b);

    ieee_1609dot2_base_types_public_encryption_key codec;
    OCTETSTRING                                    is = bit2oct(b);
    if (codec.decode(is, p_public_encryption_key) == -1) {
      loggers::get_instance().warning("fx__dec__PublicEncryptionKey: -1 result code was returned");
      return -1;
    }

    loggers::get_instance().log_msg("<<< fx__dec__PublicEncryptionKey: ", p_public_encryption_key);
    return 0;
  }

  BITSTRING fx__enc__CtlFormat(EtsiTs102941TrustLists::CtlFormat const &p_ctl_format) {
    loggers::get_instance().log_msg(">>> fx__enc__CtlFormat: ", p_ctl_format);

    etsi_ts102941_trust_lists_ctl_format codec;
    OCTETSTRING                                    os;
    if (codec.encode(p_ctl_format, os) == -1) {
      loggers::get_instance().warning("fx__enc__CtlFormat: -1 result code was returned");
      return int2bit(0, 1);
    }

    return oct2bit(os);
  }

  INTEGER fx__dec__CtlFormat(BITSTRING &b, EtsiTs102941TrustLists::CtlFormat &p_ctl_format) {
    loggers::get_instance().log_msg(">>> fx__dec__CtlFormat: ", b);

    etsi_ts102941_trust_lists_ctl_format codec;
    OCTETSTRING                          is = bit2oct(b);
    if (codec.decode(is, p_ctl_format) == -1) {
      loggers::get_instance().warning("fx__dec__CtlFormat: -1 result code was returned");
      return -1;
    }

    loggers::get_instance().log_msg("<<< fx__dec__CtlFormat: ", p_ctl_format);
    return 0;
  }

} // End of namespace LibItsPki__EncdecDeclarations
