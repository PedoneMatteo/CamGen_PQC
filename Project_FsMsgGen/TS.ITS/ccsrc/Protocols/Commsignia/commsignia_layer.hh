/*!
 * \file      udp_layer_factory.hh
 * \brief     Header file for ITS Commsignia port protocol layer factory.
 * \author    ETSI STF525
 * \copyright ETSI Copyright Notification
 *            No part may be reproduced except as authorized by written permission.
 *            The copyright and the foregoing restriction extend to reproduction in all media.
 *            All rights reserved.
 * \version   0.1
 */
#pragma once

#include "params.hh"
#include "t_layer.hh"

class OCTETSTRING; //! Forward declaration of TITAN class

/*!
 * \class commsignia_layer
 * \brief  This class provides description of ITS Commsignia port protocol layer
 */
class commsignia_layer : public layer {
  static constexpr unsigned char LL_ADDR_LENGTH     = 6;
  static constexpr unsigned char LL_ORG_CODE_LENGTH = 3;

  /**
   * \struct C2P packet description
   */
  typedef struct {
    unsigned char u8_ver_type;
    unsigned int  u32_tst_sec;
    unsigned int  u32_tst_msec;
  } __attribute__((__packed__)) c2p_s_header_t;
  typedef struct {
    c2p_s_header_t s_header;
    unsigned char  u8_primary_channel;
    unsigned char  u8_secondary_channel;
    unsigned char  u8_used_interface;
    unsigned char  u8_data_rate;
    unsigned char  u8_antenna;
    int            s32_latitude;
    int            s32_longitude;
    unsigned short u16_speed;
    unsigned short u16_heading;
    signed char    s8_rssi_ant_1;
    signed char    s8_rssi_ant_2;
    signed char    s8_noise_ant_1;
    signed char    s8_noise_ant_2;
    unsigned short u16_cbr_ant_1;
    unsigned short u16_cbr_ant_2;
  } __attribute__((__packed__)) c2p_s_v1_rx_t;
  typedef struct {
    c2p_s_header_t s_header;
    unsigned char  u8_primary_channel;
    unsigned char  u8_secondary_channel;
    unsigned char  u8_used_interface;
    unsigned char  u8_data_rate;
    unsigned char  u8_antenna;
    int            s32_latitude;
    int            s32_longitude;
    unsigned short u16_speed;
    unsigned short u16_heading;
    signed char    s8_txp;
    signed char    s8_rssi_ant_1;
    signed char    s8_rssi_ant_2;
  } __attribute__((__packed__)) c2p_s_v1_tx_t;
  typedef struct {
    c2p_s_header_t s_header;
    unsigned char  u8_socket_index;
    unsigned short u16_ethertype;
    signed char    s8_rssi;
    unsigned char  u8_datarate_500kbps;
  } __attribute__((__packed__)) c2p_s_v1_rx_cv2x_t;
  typedef struct {
    c2p_s_header_t s_header;
    unsigned char  u8_b_sps;
    unsigned short u16_sps_port;
    unsigned short u16_event_port;
    int            s32_tx_pwr_dbm;
    int            s32_bw_res_v2xid;
    int            s32_bw_res_period_interval_ms;
    int            s32_bw_res_tx_reservation_size_bytes;
    unsigned char  u8_bw_res_tx_priority;
  } __attribute__((__packed__)) c2p_s_v1_tx_cv2x_t;

  /**
   * \struct Injected data header
   */
  typedef struct {
    unsigned short frame_ctrl;
    unsigned short duration;
    unsigned char  dst_addr[commsignia_layer::LL_ADDR_LENGTH];
    unsigned char  src_addr[commsignia_layer::LL_ADDR_LENGTH];
    unsigned char  bss_id[commsignia_layer::LL_ADDR_LENGTH];
    unsigned short fragment_seq_num;
  } __attribute__((__packed__)) c2p_802_11p_hdr;

  typedef unsigned short c2p_qos_ctrl;

  /**
   * \struct C2P/c2p_s_llc_hdr_t
   */
  typedef struct {
    unsigned char  dsap;
    unsigned char  ssap;
    unsigned char  ctrl;
    unsigned char  org_code[commsignia_layer::LL_ORG_CODE_LENGTH];
    unsigned short type;
  } __attribute__((__packed__)) c2p_llc_hdr;

  params                     _params;   //! Layer parameters
  std::vector<unsigned char> _mac_src;  //! Used to optimize filtering on source mac address in \see commsignia_layer::receive_data method
  std::vector<unsigned char> _eth_type; //! Used to optimize filtering on ethernet type in \see commsignia_layer::receive_data method

  //! Fixed header for packet injection
  static unsigned char _g5_fixed_header[10];
  static unsigned char _cv2x_fixed_header[10];

public:
  //! \publicsection
  /*!
   * \brief Specialised constructor
   *        Create a new instance of the commsignia_layer class
   * \param[in] p_type \todo
   * \param[in] p_param \todo
   */
  commsignia_layer(const std::string &p_type, const std::string &param);
  /*!
   * \brief Default destructor
   * \remark If \see _reconnect_on_send is set to false, the disconnection is done by the destructor
   */
  virtual ~commsignia_layer(){};

  virtual void send_data(OCTETSTRING &data, params &params);
  virtual void receive_data(OCTETSTRING &data, params &info);
}; // End of class commsignia_layer
