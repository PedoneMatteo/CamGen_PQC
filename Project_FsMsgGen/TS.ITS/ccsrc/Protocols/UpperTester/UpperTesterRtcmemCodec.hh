#ifndef UpperTesterRtcmemCODEC_H
#define UpperTesterRtcmemCODEC_H

#include <memory>

#include "codec.hh"
#include "params.hh"

class Base_Type;
class Record_Type;
class TTCN_Typedescriptor_t;
class TTCN_Buffer;

namespace LibItsRtcmem__TypesAndValues {
  class UtRtcmemResults;
  class UtRtcmemEventInd;
  class UtSsemEventInd;
} // namespace LibItsRtcmem__TypesAndValues

class UpperTesterRtcmemCodec : public codec<Record_Type, Record_Type> {
public:
  static unsigned char c_utRtcmemInitialize;
  static unsigned char c_utRtcmemInitializeResult;
  static unsigned char c_utRtcmemTrigger;
  static unsigned char c_utRtcmemTriggerResult;
  static unsigned char c_utRtcmemUpdate;
  static unsigned char c_utRtcmemUpdateResult;
  static unsigned char c_utRtcmemTermination;
  static unsigned char c_utRtcmemTerminationResult;
  static unsigned char c_utRtcmemEventIndication;

  int encode_(const Base_Type &type, const TTCN_Typedescriptor_t &field_descriptor, TTCN_Buffer &encoding_buffer);

public:
  UpperTesterRtcmemCodec() : codec<Record_Type, Record_Type>(){};
  virtual ~UpperTesterRtcmemCodec(){};

  virtual int                encode(const Record_Type &, OCTETSTRING &data);
  virtual int                decode(const OCTETSTRING &data, Record_Type &, params *params = NULL);
  std::unique_ptr<Base_Type> decode(const OCTETSTRING &data, params *params = NULL);

private:
  int decode(const OCTETSTRING &data, LibItsRtcmem__TypesAndValues::UtRtcmemResults &msg, params *params = NULL);
  int decode(const OCTETSTRING &data, LibItsRtcmem__TypesAndValues::UtRtcmemEventInd &msg, params *params = NULL);
};
#endif
