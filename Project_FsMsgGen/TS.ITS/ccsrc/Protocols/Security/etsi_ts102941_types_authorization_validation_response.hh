#pragma once

#include "codec.hh"
#include "params.hh"

#include "EtsiTs102941TypesAuthorizationValidation.hh"

class etsi_ts102941_types_authorization_validation_response : public codec<EtsiTs102941TypesAuthorizationValidation::AuthorizationValidationResponse,
                                                                           EtsiTs102941TypesAuthorizationValidation::AuthorizationValidationResponse> {
public:
  explicit etsi_ts102941_types_authorization_validation_response()
    : codec<EtsiTs102941TypesAuthorizationValidation::AuthorizationValidationResponse,
            EtsiTs102941TypesAuthorizationValidation::AuthorizationValidationResponse>(){};
  virtual ~etsi_ts102941_types_authorization_validation_response(){};

  virtual int encode(const EtsiTs102941TypesAuthorizationValidation::AuthorizationValidationResponse &p_authorization_validation_response, OCTETSTRING &p_data);
  virtual int decode(const OCTETSTRING &p_data, EtsiTs102941TypesAuthorizationValidation::AuthorizationValidationResponse &p_authorization_validation_response,
                     params *p_params = NULL);
}; // End of class etsi_ts102941_types_authorization_validation_response
