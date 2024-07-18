/*!
 * \file      uppertester_denm_codec.hh
 * \brief     Header file for ITS UpperTester IVIM codec definition.
 * \author    ETSI STF525
 * \copyright ETSI Copyright Notification
 *            No part may be reproduced except as authorized by written permission.
 *            The copyright and the foregoing restriction extend to reproduction in all media.
 *            All rights reserved.
 * \version   0.1
 */
#pragma once

#include <memory>

#include "codec.hh"
#include "params.hh"

class Base_Type;
class Record_Type;
class TTCN_Typedescriptor_t;
class TTCN_Buffer;

namespace LibItsIvim__TypesAndValues {
  class UtIvimResults;
  class UtIvimEventInd;
} // namespace LibItsIvim__TypesAndValues

class uppertester_ivim_codec : public codec<Record_Type, Record_Type> {
public:
  static unsigned char c_utIvimInitialize;
  static unsigned char c_utIvimInitializeResult;
  static unsigned char c_utIvimTrigger;
  static unsigned char c_utIvimTriggerResult;
  static unsigned char c_utIvimUpdate;
  static unsigned char c_utIvimUpdateResult;
  static unsigned char c_utIvimTermination;
  static unsigned char c_utIvimTerminationResult;
  static unsigned char c_utIvimEventIndication;

  int encode_(const Base_Type &type, const TTCN_Typedescriptor_t &field_descriptor, TTCN_Buffer &encoding_buffer);

public:
  uppertester_ivim_codec() : codec<Record_Type, Record_Type>(){};
  virtual ~uppertester_ivim_codec(){};

  virtual int                encode(const Record_Type &, OCTETSTRING &data);
  virtual int                decode(const OCTETSTRING &data, Record_Type &, params *params = NULL);
  std::unique_ptr<Base_Type> decode(const OCTETSTRING &data, params *params = NULL);

private:
  int decode(const OCTETSTRING &data, LibItsIvim__TypesAndValues::UtIvimResults &msg, params *params = NULL);
  int decode(const OCTETSTRING &data, LibItsIvim__TypesAndValues::UtIvimEventInd &msg, params *params = NULL);
}; // End of class uppertester_ivim_codec
