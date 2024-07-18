#pragma once

#include "ItsRSUsSimulator_TestSystem.hh"

#include "layer.hh"
#include "params.hh"

namespace ItsRSUsSimulator__TestSystem {

  class ConfigRsuSimulatorPort : public ConfigRsuSimulatorPort_BASE {
    params      _cfg_params;
    params      _layer_params;
    layer *     _layer;
    std::string _time_key;

  public:
    ConfigRsuSimulatorPort(const char *par_port_name);
    ~ConfigRsuSimulatorPort();

    void set_parameter(const char *parameter_name, const char *parameter_value);
    void receiveMsg(const BOOLEAN &p_ind, const params &p_params);
    void receiveMsg(const LibItsGeoNetworking__TypesAndValues::UtGnInitialize &p_ind, const params &p_params);
    void receiveMsg(const LibItsGeoNetworking__TypesAndValues::UtGnTrigger &p_ind, const params &p_params);
    void receiveMsg(const LibItsCam__TypesAndValues::UtCamInitialize &p_ind, const params &p_params);
    void receiveMsg(const LibItsCam__TypesAndValues::UtCamTrigger &p_ind, const params &p_params);
    void receiveMsg(const LibItsDenm__TypesAndValues::UtDenmInitialize &p_ind, const params &p_params);
    void receiveMsg(const LibItsDenm__TypesAndValues::UtDenmTrigger &p_ind, const params &p_params);
    void receiveMsg(const LibItsDenm__TypesAndValues::UtDenmUpdate &p_ind, const params &p_params);
    void receiveMsg(const LibItsDenm__TypesAndValues::UtDenmTermination &p_ind, const params &p_params);
    void receiveMsg(const LibItsPki__TypesAndValues::UtPkiInitialize &p_ind, const params &p_params);
    void receiveMsg(const LibItsPki__TypesAndValues::UtPkiTrigger &p_ind, const params &p_params);
    void receiveMsg(const LibItsIvim__TypesAndValues::UtIvimInitialize &p_ind, const params &p_params);
    void receiveMsg(const LibItsIvim__TypesAndValues::UtIvimTrigger &p_ind, const params &p_params);
    void receiveMsg(const LibItsIvim__TypesAndValues::UtIvimUpdate &p_ind, const params &p_params);
    void receiveMsg(const LibItsIvim__TypesAndValues::UtIvimTermination &p_ind, const params &p_params);
    void receiveMsg(const LibItsMapemSpatem__TypesAndValues::UtMapemSpatemInitialize &p_ind, const params &p_params);
    void receiveMsg(const LibItsMapemSpatem__TypesAndValues::UtMapemSpatemTrigger &p_ind, const params &p_params);
    void receiveMsg(const LibItsSremSsem__TypesAndValues::UtSremInitialize &p_ind, const params &p_params);
    void receiveMsg(const LibItsSremSsem__TypesAndValues::UtSremTrigger &p_ind, const params &p_params);
    void receiveMsg(const LibItsSremSsem__TypesAndValues::UtSremUpdate &p_ind, const params &p_params);
    void receiveMsg(const LibItsSremSsem__TypesAndValues::UtSremCancelation &p_ind, const params &p_params);
    void receiveMsg(const LibItsRtcmem__TypesAndValues::UtRtcmemInitialize &p_ind, const params &p_params);
    void receiveMsg(const LibItsRtcmem__TypesAndValues::UtRtcmemTrigger &p_ind, const params &p_params);
    void receiveMsg(const LibItsRtcmem__TypesAndValues::UtRtcmemUpdate &p_ind, const params &p_params);
    void receiveMsg(const LibItsRtcmem__TypesAndValues::UtRtcmemTermination &p_ind, const params &p_params);

    /*void send(const CfInitialize& send_par, const COMPONENT& destination_component);
    void send(const CfInitialize& send_par);
    void send(const CfInitialize_template& send_par, const COMPONENT& destination_component);
    void send(const CfInitialize_template& send_par);
    void send(const BOOLEAN& send_par, const COMPONENT& destination_component);
    void send(const BOOLEAN& send_par);
    void send(const BOOLEAN_template& send_par, const COMPONENT& destination_component);
    void send(const BOOLEAN_template& send_par);
    void send(const LibItsGeoNetworking__TypesAndValues::UtGnResults& send_par, const COMPONENT& destination_component);
    void send(const LibItsGeoNetworking__TypesAndValues::UtGnResults& send_par);
    void send(const LibItsGeoNetworking__TypesAndValues::UtGnResults_template& send_par, const COMPONENT& destination_component);
    void send(const LibItsGeoNetworking__TypesAndValues::UtGnResults_template& send_par);*/

  private:
    /* void Handle_Fd_Event(int fd, boolean is_readable,
       boolean is_writable, boolean is_error); */
    void Handle_Fd_Event_Error(int fd);
    void Handle_Fd_Event_Writable(int fd);
    void Handle_Fd_Event_Readable(int fd);
    /* void Handle_Timeout(double time_since_last_call); */

  protected:
    void user_map(const char *system_port);
    void user_unmap(const char *system_port);

    void user_start();
    void user_stop();

    void outgoing_send(const CfInitialize &send_par);
    void outgoing_send(const BOOLEAN &send_par);
    void outgoing_send(const LibItsGeoNetworking__TypesAndValues::UtGnResults &send_par);
    void outgoing_send(const LibItsGeoNetworking__TypesAndValues::UtGnEventInd &send_par);
    void outgoing_send(const LibItsCam__TypesAndValues::UtCamResults &send_par);
    void outgoing_send(const LibItsCam__TypesAndValues::UtCamEventInd &send_par);
    void outgoing_send(const LibItsDenm__TypesAndValues::UtDenmResults &send_par);
    void outgoing_send(const LibItsDenm__TypesAndValues::UtDenmEventInd &send_par);
    void outgoing_send(const LibItsPki__TypesAndValues::UtPkiResults &send_par);
    void outgoing_send(const LibItsPki__TypesAndValues::UtPkiTriggerInd &send_par);
    void outgoing_send(const LibItsIvim__TypesAndValues::UtIvimResults &send_par);
    void outgoing_send(const LibItsIvim__TypesAndValues::UtIvimEventInd &send_par);
    void outgoing_send(const LibItsMapemSpatem__TypesAndValues::UtMapemSpatemResults &send_par);
    void outgoing_send(const LibItsMapemSpatem__TypesAndValues::UtMapemEventInd &send_par);
    void outgoing_send(const LibItsMapemSpatem__TypesAndValues::UtSpatemEventInd &send_par);
    void outgoing_send(const LibItsSremSsem__TypesAndValues::UtSremResults &send_par);
    void outgoing_send(const LibItsSremSsem__TypesAndValues::UtSremEventInd &send_par);
    void outgoing_send(const LibItsSremSsem__TypesAndValues::UtSsemEventInd &send_par);
    void outgoing_send(const LibItsRtcmem__TypesAndValues::UtRtcmemResults &send_par);
    void outgoing_send(const LibItsRtcmem__TypesAndValues::UtRtcmemEventInd &send_par);
  }; // End of class ConfigRsuSimulatorPort

} // namespace ItsRSUsSimulator__TestSystem
