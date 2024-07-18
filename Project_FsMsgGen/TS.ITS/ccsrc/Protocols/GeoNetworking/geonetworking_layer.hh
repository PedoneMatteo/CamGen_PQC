/*!
 * \file      geonetworking_layer.hh
 * \brief     Header file for ITS GeoNetworking protocol layer.
 * \author    ETSI STF525
 * \copyright ETSI Copyright Notification
 *            No part may be reproduced except as authorized by written permission.
 *            The copyright and the foregoing restriction extend to reproduction in all media.
 *            All rights reserved.
 * \version   0.1
 */
#pragma once

#include <signal.h>
#include <time.h>
#include <unistd.h>

#include <mutex>

#include "geonetworking_codec.hh"
#include "location_table.hh"
#include "t_layer.hh"

class INTEGER;     //! Forward declaration of TITAN class
class OCTETSTRING; //! Forward declaration of TITAN class

namespace LibItsGeoNetworking__TestSystem {
  class GeoNetworkingPort; //! Forward declaration of TITAN class
  class GeoNetworkingReq;  //! Forward declaration of TITAN class
  class GeoNetworkingInd;  //! Forward declaration of TITAN class
} // namespace LibItsGeoNetworking__TestSystem

namespace LibItsGeoNetworking__TypesAndValues {
  class BasicHeader;      //! Forward declaration of TITAN class
  class GeoNetworkingPdu; //! Forward declaration of TITAN class
  class BeaconHeader;     //! Forward declaration of TITAN class
  class LongPosVector;    //! Forward declaration of TITAN class
  class GN__Address;      //! Forward declaration of TITAN class
} // namespace LibItsGeoNetworking__TypesAndValues

// TODO Change pointers into smart pointers
/*!
 * \class geonetworking_layer
 * \brief  This class provides description of ITS GeoNetworking protocol layer
 */
class geonetworking_layer : public t_layer<LibItsGeoNetworking__TestSystem::GeoNetworkingPort> {
  params                                                 _params; /*!< Layer parameters */
  geonetworking_codec                                    _codec;  /*!< GeoNetworking codec object reference. \see geonetworking_codec */
  LibItsGeoNetworking__TypesAndValues::GeoNetworkingPdu *_beacon; /*!< Data structure used for beaconing */
  LibItsGeoNetworking__TypesAndValues::GeoNetworkingPdu
    *_gbc_packet; /*!< Data structure used to encapsulate upper layer PDU into a GeoNetworking packet as a Geobroadcast */
  LibItsGeoNetworking__TypesAndValues::GeoNetworkingPdu
    *_shb_packet; /*!< Data structure used to encapsulate upper layer PDU into a GeoNetworking packet as a SHB */
  LibItsGeoNetworking__TypesAndValues::GeoNetworkingPdu
    *_tsb_packet; /*!< Data structure used to encapsulate upper layer PDU into a GeoNetworking packet as a TSB */
  LibItsGeoNetworking__TypesAndValues::GeoNetworkingPdu
    *_uni_packet; /*!< Data structure used to encapsulate upper layer PDU into a GeoNetworking packet as a Unicast */
  LibItsGeoNetworking__TypesAndValues::GeoNetworkingPdu *_ls_reply;          /*!< Data structure used to reply to an incoming LS_REQUEST */
  location_table                                         _location_table;    /*!< Location table, used when device_mode is set */
  location_table                                         _pass_beacon_table; /*!< Beaconing filter table */
  bool                                                   _device_mode;
  bool                                                   _secured_mode;
  bool                                                   _encrypted_mode;
  bool                                                   _enable_security_checks;
  std::mutex                                             _sendData; // FIXME To be removed, useless
  timer_t                                                _timerid;
  struct sigevent                                        _sev;
  struct itimerspec                                      _its;
  long long                                              _freq_nanosecs;
  sigset_t                                               _mask;
  struct sigaction                                       _sa;
  unsigned int                                           _sequence_number;
  int                                                    _latitude;
  int                                                    _longitude;
  bool                                                   _force_certificate;

  /*!
   * \brief Create and initialize a BasicHeader object
   * \return A BasicHeader object
   */
  const LibItsGeoNetworking__TypesAndValues::BasicHeader fill_basic_header() const;
  /*!
   * \brief Initialize a beacon object for a stand alone beaconing
   *        This object could be replaced in case of start_beaconing call from the Adapter Control Port
   * \param[in] p_latitude
   * \param[in] p_longitude
   * \param[in] p_address
   */
  void fill_beacon(const OCTETSTRING &p_ll_address, const INTEGER p_station_type = 5, const INTEGER p_country = 0, const INTEGER type_of_address = 1);
  /*!
   * \brief Initialize a GeoNetworking packet to send GeoNetworking Facility messages
   *        This object shall be updated before to be sent
   * \param[in] p_latitude
   * \param[in] p_longitude
   * \param[in] p_address
   */
  void fill_gbc_packet(const OCTETSTRING &p_ll_address, const INTEGER &p_geoAreaPosLatitude, const INTEGER &p_geoAreaPosLongitude, const INTEGER &p_distanceA,
                       const INTEGER &p_distanceB, const INTEGER &p_angle);
  /*!
   * \brief Initialize a GeoNetworking packet to send GeoNetworking Facility messages
   *        This object shall be updated before to be sent
   * \param[in] p_latitude
   * \param[in] p_longitude
   * \param[in] p_address
   */
  void fill_shb_packet(const OCTETSTRING &p_ll_address);
  /*!
   * \brief Initialize a GeoNetworking packet to send GeoNetworking Facility messages
   *        This object shall be updated before to be sent
   * \param[in] p_latitude
   * \param[in] p_longitude
   * \param[in] p_address
   * \param[in] p_hop_number
   * \param[in] p_max_hop_number
   */
  void fill_tsb_packet(const OCTETSTRING &p_ll_address, const int p_hop_number = 3, const int p_max_hop_limit = 5);
  /*!
   * \brief Initialize a GeoNetworking packet to send GeoNetworking Facility messages
   *        This object shall be updated before to be sent
   * \param[in] p_latitude
   * \param[in] p_longitude
   * \param[in] p_address
   * \param[in] p_hop_number
   * \param[in] p_max_hop_number
   */
  void fill_uni_packet(const OCTETSTRING &p_ll_address, const int p_hop_number = 3, const int p_max_hop_limit = 5);
  /*!
   * \brief Initialize an LS_REPLY GeoNetworking packet to reply to an LS_REQUEST
   *        This object shall be updated before to be sent
   * \param[in] p_latitude
   * \param[in] p_longitude
   * \param[in] p_address
   */
  void fill_ls_reply(const OCTETSTRING &p_ll_address);

  static void timer_irq_sigalrm_handler(int p_signal, siginfo_t *p_signal_info, void *p_uc);

  const int _signal_id = SIGTERM;

public:
  explicit geonetworking_layer()
    : t_layer<LibItsGeoNetworking__TestSystem::GeoNetworkingPort>(), _params(), _codec(), _beacon(NULL), _gbc_packet(NULL), _shb_packet(nullptr),
      _ls_reply(nullptr), _location_table(), _pass_beacon_table(), _device_mode{false}, _sendData(), _timerid{0}, _sev{0}, _its{0},
      _freq_nanosecs(0), _mask{0}, _sa{0}, _sequence_number{0} {};
  geonetworking_layer(const std::string &p_type, const std::string &p_param);
  virtual ~geonetworking_layer();

  /*!
   * \fn void sendMsg(const LibItsGeoNetworking__TestSystem::GeoNetworkingReq& p_gn_req, params& p_param);
   * \brief Send GeoNetworking message to the lower layers
   * \param[in] p_gn_req The GeoNetworking message to be sent
   * \param[in] p_params Some parameters to overwrite default value of the lower layers parameters
   */
  void sendMsg(const LibItsGeoNetworking__TypesAndValues::GeoNetworkingReq &p_gn_req, params &p_param);

  /*!
   * \virtual
   * \fn void send_data(OCTETSTRING& data, params& params);
   * \brief Send bytes formated data to the lower layers
   * \param[in] p_data The data to be sent
   * \param[in] p_params Some parameters to overwrite default value of the lower layers parameters
   */
  virtual void send_data(OCTETSTRING &data, params &params);
  /*!
   * \virtual
   * \fn void receive_data(OCTETSTRING& data, params& params);
   * \brief Receive bytes formated data from the lower layers
   * \param[in] p_data The bytes formated data received
   * \param[in] p_params Some lower layers parameters values when data was received
   */
  virtual void receive_data(OCTETSTRING &data, params &info);

  /*!
   * \virtual
   * \fn void send_data(OCTETSTRING& data, params& params);
   * \brief Send bytes formated data to the lower layers
   * \param[in] p_data The data to be sent
   * \param[in] p_params Some parameters to overwrite default value of the lower layers parameters
   */
  virtual OCTETSTRING trigger_ac_event(OCTETSTRING &data, params &params);

  const LibItsGeoNetworking__TypesAndValues::LongPosVector *get_lpv(const LibItsGeoNetworking__TypesAndValues::GN__Address &p_gn_address);
  void                                                      start_beaconing();
  void                                                      start_beaconing(const LibItsGeoNetworking__TypesAndValues::GeoNetworkingPdu &p_beacon);
  void                                                      stop_beaconing();
  void                                                      start_pass_beaconing(const LibItsGeoNetworking__TypesAndValues::BeaconHeader &p_beacon);
  void                                                      stop_pass_beaconing();

  int enable_secured_mode(const std::string &p_certificate_id, const boolean p_enforce_security = false);
  int disable_secured_mode();

private:
  void init(const std::string &p_type, const std::string &p_param);
  void send_beacon();
  int  build_geonetworking_pdu(OCTETSTRING &data, params &params);
  int  build_secured_pdu(OCTETSTRING &data, params &params);
  int  decode_basic_header(const OCTETSTRING &p_data, LibItsGeoNetworking__TypesAndValues::BasicHeader &p_basic_header);
  int  setup_secured_mode();

}; // End of class geonetworking_layer
