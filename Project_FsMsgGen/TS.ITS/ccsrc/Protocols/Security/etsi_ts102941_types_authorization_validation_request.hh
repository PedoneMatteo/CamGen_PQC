#pragma once

#include "codec.hh"
#include "params.hh"

#include "EtsiTs102941TypesAuthorizationValidation.hh"

class etsi_ts102941_types_authorization_validation_request : public codec<EtsiTs102941TypesAuthorizationValidation::AuthorizationValidationRequest,
                                                                          EtsiTs102941TypesAuthorizationValidation::AuthorizationValidationRequest> {
public:
  explicit etsi_ts102941_types_authorization_validation_request()
    : codec<EtsiTs102941TypesAuthorizationValidation::AuthorizationValidationRequest,
            EtsiTs102941TypesAuthorizationValidation::AuthorizationValidationRequest>(){};
  virtual ~etsi_ts102941_types_authorization_validation_request(){};

  virtual int encode(const EtsiTs102941TypesAuthorizationValidation::AuthorizationValidationRequest &p_authorization_validation_request, OCTETSTRING &p_data);
  virtual int decode(const OCTETSTRING &p_data, EtsiTs102941TypesAuthorizationValidation::AuthorizationValidationRequest &p_authorization_validation_request,
                     params *p_params = NULL);
}; // End of class etsi_ts102941_types_authorization_validation_request
