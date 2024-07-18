#pragma once

#include <map>

#include "uu_interface.hh"

#include "data_event_notifier.hh"

typedef enum MqttState: unsigned char {
  idle,
  await_connect_ack,
  await_subscribe_ack,
  publishing,
  error
} MqttState;

typedef unsigned short MessageId;

class uu_mqtt: public uu_interface, public data_event_notifier {
protected:
  static MessageId message_id_counter;
  std::string _uu_transport;
  std::string _protocol_name;
  std::string _client_id;
  MqttState _state;
  std::map<std::string, std::string> _subscribed_topics;
  std::map<std::string, std::string> _topics;
  std::map<std::string, std::string>::iterator _subscribed_topics_it;
  std::map<std::string, std::string>::const_iterator _topics_subscribe_it;

public:
  uu_mqtt();
  ~uu_mqtt();

  int setup(const params &params);
  int shutdown();

  void send_data(OCTETSTRING &data, params &params);
  void receive_data(OCTETSTRING &p_data, params &p_params);

  void update(OCTETSTRING &p_data, params &p_params);

  inline void incoming_packet_observer_attach(data_event_notifier* p_observer) {
    _observers.push_back(p_observer);
  };

  inline void incoming_packet_observer_detach(data_event_notifier* p_observer) {
    _observers.erase(std::remove(_observers.begin(), _observers.end(), p_observer), _observers.end());
  };

  inline void incoming_packet_notify(OCTETSTRING &p_data, params &p_params) {
    for (auto o : _observers) {
      o->update(p_data, p_params);
    }
  };

private:
  void close();
  void send_connect_command();
  void send_first_subscribe_command();
  void send_next_subscribe_command();
  void send_subscribe_command();
  void send_publish_command(const OCTETSTRING &p_data, const params &p_params);
  void send_disconnect_command();

  int recv_connect_ack(OCTETSTRING &p_data, params &p_params);
  int recv_subscribe_ack(OCTETSTRING &p_data, params &p_params);
  int recv_publish(OCTETSTRING &p_data, params &p_params, OCTETSTRING& p_message);

  OCTETSTRING calculate_message_length(const int p_payload_len);
  int extract_message_length(OCTETSTRING &p_data, unsigned int* p_counter);
}; // End of class uu_mqtt
