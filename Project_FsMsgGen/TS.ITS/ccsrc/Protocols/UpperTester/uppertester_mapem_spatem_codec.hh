/*!
 * \file      uppertester_mapem_spatem_codec.hh
 * \brief     Header file for ITS UpperTester MAPEM/SPATEM codec definition.
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

namespace LibItsMapemSpatem__TypesAndValues {
  class UtMapemSpatemResults;
  class UtMapemEventInd;
  class UtSpatemEventInd;
} // namespace LibItsMapemSpatem__TypesAndValues

class uppertester_mapem_spatem_codec : public codec<Record_Type, Record_Type> {
public:
  static unsigned char c_utMapemSpatemInitialize;
  static unsigned char c_utMapemSpatemInitializeResult;
  static unsigned char c_utMapemSpatemTrigger;
  static unsigned char c_utMapemSpatemTriggerResult;
  static unsigned char c_utMapemEventIndication;
  static unsigned char c_utSpatemEventIndication;

  int encode_(const Base_Type &type, const TTCN_Typedescriptor_t &field_descriptor, TTCN_Buffer &encoding_buffer);

public:
  uppertester_mapem_spatem_codec() : codec<Record_Type, Record_Type>(){};
  virtual ~uppertester_mapem_spatem_codec(){};

  virtual int                encode(const Record_Type &, OCTETSTRING &data);
  virtual int                decode(const OCTETSTRING &data, Record_Type &, params *params = NULL);
  std::unique_ptr<Base_Type> decode(const OCTETSTRING &data, params *params = NULL);

private:
  int decode(const OCTETSTRING &data, LibItsMapemSpatem__TypesAndValues::UtMapemSpatemResults &msg, params *params = NULL);
  int decode(const OCTETSTRING &data, LibItsMapemSpatem__TypesAndValues::UtMapemEventInd &msg, params *params = NULL);
  int decode(const OCTETSTRING &data, LibItsMapemSpatem__TypesAndValues::UtSpatemEventInd &msg, params *params = NULL);
}; // End of class uppertester_mapem_spatem_codec
