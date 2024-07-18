#pragma once

#include "params.hh"

class OCTETSTRING; //! Forward declaration of TITAN class

class data_event_notifier {
public:
  virtual ~data_event_notifier() = default;
  virtual void update(OCTETSTRING &p_data, params &p_params) {};
}; // End of abstract class data_event_notifier
