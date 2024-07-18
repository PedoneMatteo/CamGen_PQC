#pragma once

#include "codec.hh"
#include "params.hh"

#include "EtsiTs102941MessagesCa.hh"

class etsi_ts102941_data_codec : public codec<EtsiTs102941MessagesCa::EtsiTs102941Data, EtsiTs102941MessagesCa::EtsiTs102941Data> {
public:
  explicit etsi_ts102941_data_codec() : codec<EtsiTs102941MessagesCa::EtsiTs102941Data, EtsiTs102941MessagesCa::EtsiTs102941Data>(){};
  virtual ~etsi_ts102941_data_codec(){};

  virtual int encode(const EtsiTs102941MessagesCa::EtsiTs102941Data &p_etsi_ts_10291_data, OCTETSTRING &p_data);
  virtual int decode(const OCTETSTRING &p_data, EtsiTs102941MessagesCa::EtsiTs102941Data &p_etsi_ts_10291_data, params *p_params = NULL);
}; // End of class etsi_ts102941_data_codec
