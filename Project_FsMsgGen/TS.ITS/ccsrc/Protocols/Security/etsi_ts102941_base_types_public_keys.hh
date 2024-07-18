#pragma once

#include "codec.hh"
#include "params.hh"

#include "EtsiTs102941BaseTypes.hh"

class etsi_ts102941_base_types_public_keys : public codec<EtsiTs102941BaseTypes::PublicKeys, EtsiTs102941BaseTypes::PublicKeys> {
public:
  explicit etsi_ts102941_base_types_public_keys() : codec<EtsiTs102941BaseTypes::PublicKeys, EtsiTs102941BaseTypes::PublicKeys>(){};
  virtual ~etsi_ts102941_base_types_public_keys(){};

  virtual int encode(const EtsiTs102941BaseTypes::PublicKeys &p_public_keys, OCTETSTRING &p_data);
  virtual int decode(const OCTETSTRING &p_data, EtsiTs102941BaseTypes::PublicKeys &p_public_keys, params *p_params = NULL);
}; // End of class etsi_ts102941_base_types_public_keys
