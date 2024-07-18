#pragma once

#include <vector>

#include "data_event_notifier.hh"

class OCTETSTRING; //! Forward declaration of TITAN class

class data_event_observer {
protected:
  std::vector<data_event_notifier *> _observers;

public:
  data_event_observer() : _observers() {};
  virtual ~data_event_observer() { _observers.clear(); };
  virtual void incoming_packet_observer_attach(data_event_notifier* p_observer) {};
  virtual void incoming_packet_observer_detach(data_event_notifier* p_observer) {};
  virtual void incoming_packet_notify(OCTETSTRING &p_data, params &p_params) {};
}; // End of abstract class data_event_observer
