#ifndef UpperTesterSremSsemCODEC_H
#define UpperTesterSremSsemCODEC_H

#include <memory>

#include "codec.hh"
#include "params.hh"

class Base_Type;
class Record_Type;
class TTCN_Typedescriptor_t;
class TTCN_Buffer;

namespace LibItsSremSsem__TypesAndValues {
  class UtSremResults;
  class UtSremEventInd;
  class UtSsemEventInd;
} // namespace LibItsSremSsem__TypesAndValues

class UpperTesterSremSsemCodec : public codec<Record_Type, Record_Type> {
public:
  static unsigned char c_utSremInitialize;
  static unsigned char c_utSremInitializeResult;
  static unsigned char c_utSremTrigger;
  static unsigned char c_utSremTriggerResult;
  static unsigned char c_utSremUpdate;
  static unsigned char c_utSremUpdateResult;
  static unsigned char c_utSremCancelation;
  static unsigned char c_utSremCancelationResult;
  static unsigned char c_utSremEventIndication;
  static unsigned char c_utSsemEventIndication;

  int encode_(const Base_Type &type, const TTCN_Typedescriptor_t &field_descriptor, TTCN_Buffer &encoding_buffer);

public:
  UpperTesterSremSsemCodec() : codec<Record_Type, Record_Type>(){};
  virtual ~UpperTesterSremSsemCodec(){};

  virtual int                encode(const Record_Type &, OCTETSTRING &data);
  virtual int                decode(const OCTETSTRING &data, Record_Type &, params *params = NULL);
  std::unique_ptr<Base_Type> decode(const OCTETSTRING &data, params *params = NULL);

private:
  int decode(const OCTETSTRING &data, LibItsSremSsem__TypesAndValues::UtSremResults &msg, params *params = NULL);
  int decode(const OCTETSTRING &data, LibItsSremSsem__TypesAndValues::UtSremEventInd &msg, params *params = NULL);
  int decode(const OCTETSTRING &data, LibItsSremSsem__TypesAndValues::UtSsemEventInd &msg, params *params = NULL);
};
#endif
