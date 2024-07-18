#pragma once

#include "params.hh"
#include "layer.hh"

#include "data_event_observer.hh"

class uu_interface: public layer, public data_event_observer {
protected:
  layer* _layer;

public:
  uu_interface(): _layer{0} { };
  virtual int setup(const params &params) = 0;
  virtual int shutdown() = 0;

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

}; // End of interface uu_interface
