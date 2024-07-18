/*!
 * \file      base_time.hh
 * \brief     Header file for the control port base_time functionality.
 * \author    ETSI STF525
 * \copyright ETSI Copyright Notification
 *            No part may be reproduced except as authorized by written permission.
 *            The copyright and the foregoing restriction extend to reproduction in all media.
 *            All rights reserved.
 * \version   0.1
 */
#pragma once

#include <chrono>

/**
 * \class base_time
 * \brief This class provides time tools such as getting current time
 */
class base_time {
  const unsigned long long its_base_time_ms = 1072915200000L; //! Base time 01/01/2004 12:00am in millseconds

  unsigned long long leap_delay;

  static base_time *_instance;

private:
  base_time() : leap_delay{0} {}; //! Can not be created manually
public:
  static inline base_time &get_instance();

  virtual ~base_time() {
    if (_instance != nullptr)
      delete _instance;
  };

public:
  inline const unsigned long long get_current_time_ms() const;
  inline const unsigned long long get_its_base_time_ms() const;
  inline const unsigned long long get_its_current_time_ms() const;
  inline const unsigned long long get_its_current_time_us() const;
  inline const unsigned long long get_its_current_time_mod_ms() const;
  inline void                     set_leap_delay_us(const unsigned long long p_leap_delay);
  inline const unsigned long long get_leap_delay_us() const;
}; // End of class base_time

// static functions
base_time &base_time::get_instance() { return (_instance != nullptr) ? *_instance : *(_instance = new base_time()); }

const unsigned long long base_time::get_current_time_ms() const {
  return (leap_delay / 1000) + std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

const unsigned long long base_time::get_its_base_time_ms() const { return base_time::its_base_time_ms; }

const unsigned long long base_time::get_its_current_time_ms() const {
  return (leap_delay / 1000) + std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count() -
         base_time::its_base_time_ms;
}

const unsigned long long base_time::get_its_current_time_us() const {
  return leap_delay + std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count() -
         base_time::its_base_time_ms * 1000;
}

const unsigned long long base_time::get_its_current_time_mod_ms() const {
  return ((leap_delay / 1000) + std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count() -
          base_time::its_base_time_ms) %
         65536;
}

void base_time::set_leap_delay_us(const unsigned long long p_leap_delay) { leap_delay = p_leap_delay; }

inline const unsigned long long base_time::get_leap_delay_us() const { return leap_delay; }
