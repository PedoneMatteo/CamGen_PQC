#pragma once

#include "codec.hh"
#include "params.hh"

#include "EtsiTs102941TypesAuthorization.hh"

class etsi_ts102941_types_authorization_inner_response
  : public codec<EtsiTs102941TypesAuthorization::InnerAtResponse, EtsiTs102941TypesAuthorization::InnerAtResponse> {
public:
  explicit etsi_ts102941_types_authorization_inner_response()
    : codec<EtsiTs102941TypesAuthorization::InnerAtResponse, EtsiTs102941TypesAuthorization::InnerAtResponse>(){};
  virtual ~etsi_ts102941_types_authorization_inner_response(){};

  virtual int encode(const EtsiTs102941TypesAuthorization::InnerAtResponse &p_inner_response, OCTETSTRING &p_data);
  virtual int decode(const OCTETSTRING &p_data, EtsiTs102941TypesAuthorization::InnerAtResponse &p_inner_response, params *p_params = NULL);
}; // End of class etsi_ts102941_types_authorization_inner_response
