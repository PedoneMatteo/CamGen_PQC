/*!
 * \file      params.hh
 * \brief     Header file for the parameter dictionary.
 * \author    ETSI STF525
 * \copyright ETSI Copyright Notification
 *            No part may be reproduced except as authorized by written permission.
 *            The copyright and the foregoing restriction extend to reproduction in all media.
 *            All rights reserved.
 * \version   0.1
 */
#pragma once

#include <map>
#include <string>
#include <vector>

/*!
 * \class params
 * \brief This class provides basic functionalities for an ITS dictionary
 * \implements std::map
 */
class params : public std::map<std::string, std::string> {
public: //! \publicsection
  // TODO Use static constexpr (see commsignia_layer.hh)
  static const std::string& debug; //! Set to 1 to enable the debug mode

  static const std::string& loopback;

  static const std::string& mac_src;   //! Source MAC address parameter name
  static const std::string& mac_dst;   //! Destination MAC address parameter name
  static const std::string& mac_bc;    //! Broadcast MAC address parameter name
  static const std::string& eth_type;  //! Ethernet type parameter name
  static const std::string& filter;    //! Additinal PCAP filter
  static const std::string& beaconing; //! Beaconing mode parameter name
  static const std::string& timestamp; //! Packet reception timestamp
  static const std::string& station_type;
  static const std::string& country;
  static const std::string& type_of_address;
  static const std::string& ssp;                  //! SSP parameter name
  static const std::string& its_aid;              //! ITS-AID parameter name
  static const std::string& gn_payload;           //! GeoNetworking Payload parameter name
  static const std::string& gn_next_header;       //! GeoNetworking NextHeader parameter name
  static const std::string& gn_header_type;       //! GeoNetworking HeaderType parameter name
  static const std::string& gn_header_sub_type;   //! GeoNetworking HeaderSubType parameter name
  static const std::string& gn_lifetime;          //! GeoNetworking Lifetime parameter name
  static const std::string& gn_traffic_class;     //! GeoNetworking Traffic class parameter name
  static const std::string& btp_type;             //! BTP Type parameter name
  static const std::string& btp_payload;          //! BTP Payload parameter name
  static const std::string& btp_destination_port; //! BTP DestinationPort parameter name
  static const std::string& btp_info;             //! BTP Info parameter name

  static const std::string& nic;        //! Network Interface Card parameter name
  static const std::string& ll_address; //! Test system GeoNetworking LL-Address parameter name
  static const std::string& latitude;   //! Test system Latitude parameter name
  static const std::string& longitude;  //! Test system Longitude parameter name
  static const std::string& expiry;     //! Test system GeoNetworking Lifetime parameter name (in ms)

  static const std::string& station_id; //! To indicate to the peer ITS_Container::stationID

  static const std::string& device_mode;  //! To indicate to the lower layer to act as a standalone device
  static const std::string& secured_mode; //! To indicate to the lower layer to apply signature on message exchanges as defined in IEEE 1609.2&  ETSI TS 102 965
  static const std::string
   & encrypted_mode; //! To indicate to the lower layer to apply encryption on message exchanges as defined in IEEE 1609.2&  ETSI TS 102 965
  static const std::string& enable_security_checks; //! To indicates if security check failures shall be treated as error or warning
  static const std::string& sec_db_path;            //! Path to the folder containing certificates
  static const std::string& force_certificate;      //! Force certificate instead of hashed_id into next message
  static const std::string& certificate;            //! The certificate identifier the Test System shall use. E.g. CERT_TS_A
  static const std::string& peer_certificate;       //! The peer certificate identifier the Test System shall use for encryption. E.g. CERT_TS_A
  static const std::string& hash;      //! The digest algorithm the Test System shall use, authorised values are SHA-256 or SHA-384. Default: SHA-256
  static const std::string& signature; //! The signature algorithm the Test System shall use, authorised values are NISTP-256, BP-256 and BP-384. Default: NISTP-256
  static const std::string& cypher;    //! The encryption algorithm the Test System shall use, authorised values are NISTP-256 and BP-256. Default: NISTP-256
  static const std::string& distanceA; //! Test system GeoNetworking DistanceA parameter name
  static const std::string& distanceB; //! Test system GeoNetworking DistanceB parameter name
  static const std::string& angle;     //! Test system GeoNetworking Angle parameter name

  static const std::string& payload_type; //! Tyoe of payload, value is the ITS Conatainer message identifier or omitted in case of unknown payload

  static const std::string& next_header;     //! Upper layer settings
  static const std::string& header_type;     //! Upper layer settings
  static const std::string& header_sub_type; //! Upper layer settings

  static const std::string& interface_id; //! Commsignia antenna selector

  static const std::string& uu_protocol;    //! Uu interafce protocol (raw, MQTT, AMQP). Default: raw
  static const std::string& uu_transport;   //! Uu interafce transport (TCP, UDP)
  static const std::string& pc5_layer;      //! Radio specific layer for PC5 (e.g. Qualcomm, Commsignia)
  static const std::string& mqtt_topics;    //! Uu interafce MQTT topics
  static const std::string& mqtt_protocol;  //! Uu interafce MQTT protocol name
  static const std::string& mqtt_client_id; //! Uu interafce MQTT client identity

  static const std::string& server;      //! HTTP server address (e.g. www.etsi.org)
  static const std::string& port;        //! HTTP server port. Default: 80
  static const std::string& use_ssl;     //! Set to 1 to use SSL to communicate with the HTTP server. Default: false
  static const std::string& mutual_auth; //! Set to 1 to use mutual authentication. Default: false
  static const std::string& trusted_ca_list;       //! List of trusted CA certificates
  static const std::string& server_mode; //! Does the test sytem acting as a server. Default: 0
  static const std::string& local_port;  //! Local listener port. Default: 80

  static const std::string& method;       //! HTTP method type. Default: POST
  static const std::string& uri;          //! HTTP URI value. Default: /
  static const std::string& host;         //! HTTP Host value. Default: 127.0.0.1
  static const std::string& content_type; //! HTTP Content-type value. Default: application/text

  static const std::string& codecs; //! List of codecs to use for HTTP application layers

  /*!
   * \brief Default constructor
   *        Create a new instance of the params class
   */
  params() : std::map<std::string, std::string>(){};
  /*!
   * \brief Copy constructor
   *        Clone an existing instance of a params object
   * \param[in] p_params An existing instance of a params object
   */
  explicit params(const params &p_params) : std::map<std::string, std::string>(p_params.begin(), p_params.end()){};

  /*!
   * \brief Default destructor
   */
  virtual ~params(){};

  /*!
   * \fn void log() const;
   * \brief Provides a dump of the content of this instance
   */
  void log() const;
  /*!
   * \fn void log() const;
   * \brief Provides a dump of the content of this instance
   */
  void log();
  /*!
   * \fn void reset();
   * \brief Reset the content of this instance
   */
  void reset();

  /*!
   * \static
   * \fn void convert(params& p_param, const std::string p_parameters);
   * \brief Create a new instance of a params object by converting a list of ITS parameters in string format (t1=v1,T2=(v0,v1v2)...)
   * \return a new instance of a params object
   */
  static void convert(params &p_param, const std::string p_parameters);
}; // End of class params
