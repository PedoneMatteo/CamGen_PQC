#pragma once

#include "codec.hh"
#include "params.hh"

#include "EtsiTs102941TypesAuthorization.hh"

class etsi_ts102941_types_authorization_shared_at_request
  : public codec<EtsiTs102941TypesAuthorization::SharedAtRequest, EtsiTs102941TypesAuthorization::SharedAtRequest> {
public:
  explicit etsi_ts102941_types_authorization_shared_at_request()
    : codec<EtsiTs102941TypesAuthorization::SharedAtRequest, EtsiTs102941TypesAuthorization::SharedAtRequest>(){};
  virtual ~etsi_ts102941_types_authorization_shared_at_request(){};

  virtual int encode(const EtsiTs102941TypesAuthorization::SharedAtRequest &p_shared_at_request, OCTETSTRING &p_data);
  virtual int decode(const OCTETSTRING &p_data, EtsiTs102941TypesAuthorization::SharedAtRequest &p_shared_at_request, params *p_params = NULL);
}; // End of class etsi_ts102941_types_authorization_shared_at_request
