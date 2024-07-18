#pragma once

#include "codec.hh"
#include "params.hh"

#include "Ieee1609Dot2BaseTypes.hh"

class ieee_1609dot2_base_types_public_verification_key
  : public codec<Ieee1609Dot2BaseTypes::PublicVerificationKey, Ieee1609Dot2BaseTypes::PublicVerificationKey> {
public:
  explicit ieee_1609dot2_base_types_public_verification_key()
    : codec<Ieee1609Dot2BaseTypes::PublicVerificationKey, Ieee1609Dot2BaseTypes::PublicVerificationKey>(){};
  virtual ~ieee_1609dot2_base_types_public_verification_key(){};

  virtual int encode(const Ieee1609Dot2BaseTypes::PublicVerificationKey &p_public_verification_key, OCTETSTRING &p_data);
  virtual int decode(const OCTETSTRING &p_data, Ieee1609Dot2BaseTypes::PublicVerificationKey &p_public_verification_key, params *p_params = NULL);
}; // End of class ieee_1609dot2_base_types_public_verification_key
