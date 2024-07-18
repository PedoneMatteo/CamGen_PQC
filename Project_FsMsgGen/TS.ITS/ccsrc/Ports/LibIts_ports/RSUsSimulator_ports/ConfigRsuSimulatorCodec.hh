#pragma once

#include "codec.hh"
#include "params.hh"

class Record_Type;
class Base_Type;
class TTCN_Typedescriptor_t;
class TTCN_Buffer;

namespace ItsRSUsSimulator__TestSystem {
  class CfInitialize;
}

class ConfigRsuSimulatorCodec : public codec<Record_Type, Record_Type> {
  int encode_(const Base_Type &type, const TTCN_Typedescriptor_t &field_descriptor, TTCN_Buffer &encoding_buffer);

public:
  explicit ConfigRsuSimulatorCodec() : codec<Record_Type, Record_Type>(){};
  virtual ~ConfigRsuSimulatorCodec(){};

  virtual int encode(const Record_Type &, OCTETSTRING &data);
  virtual int decode(const OCTETSTRING &data, Record_Type &, params *params = NULL);
}; // End of class ConfigRsuSimulatorCodec
