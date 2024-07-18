#pragma once

#include "codec.hh"
#include "params.hh"

#include "EtsiTs102941TypesAuthorization.hh"

class etsi_ts102941_types_authorization_inner_request
  : public codec<EtsiTs102941TypesAuthorization::InnerAtRequest, EtsiTs102941TypesAuthorization::InnerAtRequest> {
public:
  explicit etsi_ts102941_types_authorization_inner_request()
    : codec<EtsiTs102941TypesAuthorization::InnerAtRequest, EtsiTs102941TypesAuthorization::InnerAtRequest>(){};
  virtual ~etsi_ts102941_types_authorization_inner_request(){};

  virtual int encode(const EtsiTs102941TypesAuthorization::InnerAtRequest &p_inner_request, OCTETSTRING &p_data);
  virtual int decode(const OCTETSTRING &p_data, EtsiTs102941TypesAuthorization::InnerAtRequest &p_inner_request, params *p_params = NULL);
}; // End of class etsi_ts102941_types_authorization_inner_request
