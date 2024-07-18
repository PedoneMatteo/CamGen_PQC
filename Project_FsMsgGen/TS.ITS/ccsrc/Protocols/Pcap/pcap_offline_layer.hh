/*!
 * \file      pcap_offline_layer.hh
 * \brief     Header file for ITS Offline Pcap port layer.
 * \author    ETSI STF525
 * \copyright ETSI Copyright Notification
 *            No part may be reproduced except as authorized by written permission.
 *            The copyright and the foregoing restriction extend to reproduction in all media.
 *            All rights reserved.
 * \version   0.1
 */
#pragma once

#include <mutex>
#include <thread>

#include "params.hh"
#include "t_layer.hh"

#include <Octetstring.hh>

class PORT; //! Forward declaration of TITAN class

typedef struct pcap pcap_t;

/*!
 * \class pcap_layer
 * \brief  This class provides description of ITS PCAP port protocol layer
 */
class pcap_offline_layer : public layer, public PORT {
  params       _params; //! Layer parameters
  pcap_t *     _device; //! Device handle
  std::thread *_thread; //! Thread handle, used to read PCAP file instead of NIC, used in file mode
  std::mutex   _resume;
  bool         _running;  //! Set to true when the thread is running, used in file mode
  bool         _realtime; //! Set to true if realtime delay shall be added between packets
  bool         _loop;     //! Set to true if playback shall be looped
  int          _fd[2];    //! pipe to signal to Titan

  params      _o_params;
  OCTETSTRING _o_data;

  std::string _time_key;

  static void *run(void *p_this);

public:
  void *thread(void);

public: //! \publicsection
  /*!
   * \brief Specialised constructor
   *        Create a new instance of the pcap_layer class
   * \param[in] p_type \todo
   * \param[in] p_param \todo
   */
  pcap_offline_layer(const std::string &p_type, const std::string &param);
  /*!
   * \brief Default destructor
   */
  virtual ~pcap_offline_layer();

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

  void Handle_Fd_Event_Readable(int fd);
};
