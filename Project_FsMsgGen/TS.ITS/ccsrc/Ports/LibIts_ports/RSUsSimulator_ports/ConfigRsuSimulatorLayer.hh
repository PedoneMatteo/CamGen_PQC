/*!
 * \file      ConfigRsuSimulatorLayer.hh
 * \brief     Header file for ITS Configuration port protocol layer.
 * \author    ETSI STF525
 * \copyright ETSI Copyright Notification
 *            No part may be reproduced except as authorized by written permission.
 *            The copyright and the foregoing restriction extend to reproduction in all media.
 *            All rights reserved.
 * \version   0.1
 */
#pragma once

#include "ConfigRsuSimulatorCodec.hh"
#include "UpperTesterRtcmemCodec.hh"
#include "UpperTesterSremSsemCodec.hh"
#include "t_layer.hh"
#include "uppertester_cam_codec.hh"
#include "uppertester_denm_codec.hh"
#include "uppertester_geonetworking_codec.hh"
#include "uppertester_ivim_codec.hh"
#include "uppertester_mapem_spatem_codec.hh"
#include "uppertester_pki_codec.hh"

namespace ItsRSUsSimulator__TestSystem {
  class ConfigRsuSimulatorPort;
  class CfInitialize;
} // namespace ItsRSUsSimulator__TestSystem

class ConfigRsuSimulatorLayer : public t_layer<ItsRSUsSimulator__TestSystem::ConfigRsuSimulatorPort> {
  params                  _params;
  ConfigRsuSimulatorCodec _codec;
  uppertester_cam_codec   _codec_cam; // FIXME Enhance all codec to encode inputs messages also

public:
  ConfigRsuSimulatorLayer() : t_layer<ItsRSUsSimulator__TestSystem::ConfigRsuSimulatorPort>(), _params(), _codec(), _codec_cam(){};
  ConfigRsuSimulatorLayer(const std::string &p_type, const std::string &param);
  virtual ~ConfigRsuSimulatorLayer(){};

  void sendMsg(const ItsRSUsSimulator__TestSystem::CfInitialize &, params &params);
  void sendMsg(const LibItsGeoNetworking__TypesAndValues::UtGnResults &send_par, params &params);
  void sendMsg(const LibItsGeoNetworking__TypesAndValues::UtGnEventInd &send_par, params &params);
  void sendMsg(const LibItsCam__TypesAndValues::UtCamResults &send_par, params &params);
  void sendMsg(const LibItsCam__TypesAndValues::UtCamEventInd &send_par, params &params);
  void sendMsg(const LibItsDenm__TypesAndValues::UtDenmResults &send_par, params &params);
  void sendMsg(const LibItsDenm__TypesAndValues::UtDenmEventInd &send_par, params &params);
  void sendMsg(const LibItsPki__TypesAndValues::UtPkiResults &send_par, params &params);
  void sendMsg(const LibItsPki__TypesAndValues::UtPkiTriggerInd &send_par, params &params);
  void sendMsg(const LibItsIvim__TypesAndValues::UtIvimResults &send_par, params &params);
  void sendMsg(const LibItsIvim__TypesAndValues::UtIvimEventInd &send_par, params &params);
  void sendMsg(const LibItsMapemSpatem__TypesAndValues::UtMapemSpatemResults &send_par, params &params);
  void sendMsg(const LibItsMapemSpatem__TypesAndValues::UtMapemEventInd &send_par, params &params);
  void sendMsg(const LibItsMapemSpatem__TypesAndValues::UtSpatemEventInd &send_par, params &params);
  void sendMsg(const LibItsSremSsem__TypesAndValues::UtSremResults &send_par, params &params);
  void sendMsg(const LibItsSremSsem__TypesAndValues::UtSremEventInd &send_par, params &params);
  void sendMsg(const LibItsSremSsem__TypesAndValues::UtSsemEventInd &send_par, params &params);
  void sendMsg(const LibItsRtcmem__TypesAndValues::UtRtcmemResults &send_par, params &params);
  void sendMsg(const LibItsRtcmem__TypesAndValues::UtRtcmemEventInd &send_par, params &params);

  virtual void send_data(OCTETSTRING &data, params &params);
  virtual void receive_data(OCTETSTRING &data, params &params);

private:
  int process_utinitialize_data(const OCTETSTRING &data, params &params);
  int process_ut_cam_changespeed_data(const OCTETSTRING &data, params &params);
  int process_ut_cam_changecurvature_data(const OCTETSTRING &data, params &params);
  int process_ut_cam_changeheading_data(const OCTETSTRING &data, params &params);
  int process_ut_cam_setvehiclerole_data(const OCTETSTRING &data, params &params);
  int process_ut_denm_trigger(const OCTETSTRING &data, params &params);
  int process_ut_denm_update(const OCTETSTRING &data, params &params);
  int process_ut_denm_termination(const OCTETSTRING &data, params &params);
  int process_ut_geonetworking_trigger(const OCTETSTRING &data, params &params);
  int process_ut_pki_trigger(const OCTETSTRING &data, params &params);
  int process_ut_ivim_trigger(const OCTETSTRING &data, params &params);
  int process_ut_ivim_update(const OCTETSTRING &data, params &params);
  int process_ut_ivim_termination(const OCTETSTRING &data, params &params);
  int process_ut_mapem_spatem_trigger(const OCTETSTRING &data, params &params);
  int process_ut_srem_trigger(const OCTETSTRING &data, params &params);
  int process_ut_srem_update(const OCTETSTRING &data, params &params);
  int process_ut_srem_termination(const OCTETSTRING &data, params &params);
  int process_ut_rtcmem_trigger(const OCTETSTRING &data, params &params);
  int process_ut_rtcmem_update(const OCTETSTRING &data, params &params);
  int process_ut_rtcmem_termination(const OCTETSTRING &data, params &params);
}; // End of class ConfigRsuSimulatorLayer
