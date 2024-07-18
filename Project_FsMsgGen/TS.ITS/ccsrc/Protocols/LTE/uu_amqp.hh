#pragma once

#include "uu_interface.hh"

#include "data_event_notifier.hh"

class uu_amqp: public uu_interface, public data_event_notifier {
protected:
  std::string _uu_transport;

public:
  uu_amqp();
  ~uu_amqp();

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
}; // End of class uu_amqp
