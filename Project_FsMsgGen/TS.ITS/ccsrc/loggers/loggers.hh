/*!
 * \file      loogers.hh
 * \brief     Header file for the logger framework.
 * \author    ETSI STF525
 * \copyright ETSI Copyright Notification
 *            No part may be reproduced except as authorized by written permission.
 *            The copyright and the foregoing restriction extend to reproduction in all media.
 *            All rights reserved.
 * \version   0.1
 */
#pragma once

#include <cstdarg>
#include <ctime>
#include <map>
#include <memory>
#include <string>

/**
class Base_Type;
class OCTETSTRING;
class TTCN_Buffer;
class TTCN_Logger;
enum  TTCN_Logger::Severity;
extern void TTCN_error(const char *err_msg, ...) __attribute__ ((__format__ (__printf__, 1, 2), __noreturn__));
extern void TTCN_error_begin(const char *err_msg, ...) __attribute__ ((__format__ (__printf__, 1, 2)));
extern void TTCN_error_end() __attribute__ ((__noreturn__));
void TTCN_warning(const char *warning_msg, ...) __attribute__ ((__format__ (__printf__, 1, 2)));
extern void TTCN_warning_begin(const char *warning_msg, ...) __attribute__ ((__format__ (__printf__, 1, 2)));
extern void TTCN_warning_end();
**/

using namespace std; // Required for isnan()
#include <TTCN3.hh>

/*!
 * \class loggers
 * \brief This class provides basic functionalities for an ITS dictionary
 * \implements Singleton pattern
 * \todo Remove reference to TTCN3.hh
 */
class loggers {
private:                                     //! \privatesection
  static std::unique_ptr<loggers> _instance; //! Smart pointer to the unique instance of the logger framework
  std::map<std::string, std::clock_t>
    _times; //! Timer used to measure execution time between calls to methods \loggers::set_start_time and \loggers::set_stop_time

  /*!
   * \brief Default constructor
   *        Create a new instance of the loggers class
   * \private
   */
  explicit loggers() : _times(){};

  inline void log_time_exec(const char *p_fmt, ...);

public: //! \publicsection
  /*!
   * \brief Default destructor
   */
  virtual ~loggers(){};

  /*!
   * \fn static loggers& get_instance();
   * \brief Accessor for the unique instance of the logger framework
   * \inline
   */
  static inline loggers &get_instance() { return *_instance.get(); };

  /*!
   * \fn void log_to_hexa(const char *p_prompt, const TTCN_Buffer& buffer);
   * \brief Hexa dump of the \see TTCN_Buffer buffer
   * \param[in] p_prompt  Label of the log to be produced
   * \param[in] buffer  The TTCN_Buffer buffer to dump
   * \inline
   */
  inline void log_to_hexa(const char *p_prompt, const TTCN_Buffer &buffer);
  /*!
   * \fn void log_to_hexa(const char *p_prompt, const OCTETSTRING& msg);
   * \brief Hexa dump of the \see OCTETSTRING buffer
   * \param[in] p_prompt  Label of the log to be produced
   * \param[in] msg     The OCTETSTRING buffer to dump
   * \inline
   */
  inline void log_to_hexa(const char *p_prompt, const OCTETSTRING &msg);
  /*!
   * \fn void log_to_hexa(const char *p_prompt, const unsigned char* msg, const size_t msg_size);
   * \brief Hexa dump of the provided buffer
   * \param[in] p_prompt  Label of the log to be produced
   * \param[in] msg     The buffer to dump
   * \inline
   */
  inline void log_to_hexa(const char *p_prompt, const unsigned char *msg, const size_t msg_size);
  /*!
   * \fn void log_msg(const char *p_prompt, const Base_Type& p_type);
   * \brief Debug log of TITAN data structures
   * \param[in] p_prompt  Label of the log to be produced
   * \param[in] msg     The TITAN data structure to log
   * \inline
   */
  inline void log_msg(const char *p_prompt, const Base_Type &p_type);
  /*!
   * \fn void log(const char *p_fmt, ...);
   * \brief Debug message based on printf-compliant formatting message
   * \param[in] p_fmt  The printf-compliant format of the message to log
   * \param[in] ...  The arguments
   * \inline
   */
  inline void log(const char *p_fmt, ...);

  /*!
   * \fn void user_msg(const char *p_prompt, const Base_Type& p_type);
   * \brief User message of TITAN data structures
   * \param[in] p_prompt  Label of the log to be produced
   * \param[in] msg     The TITAN data structure to log
   * \inline
   */
  inline void user_msg(const char *p_prompt, const Base_Type &p_type);
  /*!
   * \fn void user(const char *p_fmt, ...);
   * \brief User message based on printf-compliant formatting message
   * \param[in] p_fmt  The printf-compliant format of the message to log
   * \param[in] ...  The arguments
   * \inline
   */
  inline void user(const char *p_fmt, ...);

  /*!
   * \fn void user_msg(const char *p_prompt, const Base_Type& p_type);
   * \brief Warning message of TITAN data structures
   * \param[in] p_prompt  Label of the log to be produced
   * \param[in] msg     The TITAN data structure to log
   * \inline
   */
  inline void warning_msg(const char *p_prompt, const Base_Type &p_type);
  /*!
   * \fn void user(const char *p_fmt, ...);
   * \brief Warning message based on printf-compliant formatting message
   * \param[in] p_fmt  The printf-compliant format of the message to log
   * \param[in] ...  The arguments
   * \inline
   */
  inline void warning(const char *p_fmt, ...);

  /*!
   * \fn void user(const char *p_fmt, ...);
   * \brief Error message based on printf-compliant formatting message
   * \param[in] p_fmt  The printf-compliant format of the message to log
   * \param[in] ...  The arguments
   * \inline
   */
  inline void error(const char *p_fmt, ...);

  /*!
   * \fn void set_start_time(std::string& p_time_key);
   * \brief Start execution time measurement
   * \param[in] p_time_key A timer identifier (any string)
   * \inline
   */
  inline void set_start_time(std::string &p_time_key);
  /*!
   * \fn void set_stop_time(std::string& p_time_key, float& p_time);
   * \brief Stop execution time measurement
   * \param[in] p_time_key The timer identifier provided while calling \see loggers::set_start_time method
   * \param[out] p_time    The execution time measured in milliseconds
   * \inline
   */
  inline void set_stop_time(std::string &p_time_key, float &p_time);
}; // End of class loggers

void loggers::log_to_hexa(const char *p_prompt, const TTCN_Buffer &buffer) {
  TTCN_Logger::begin_event(TTCN_Logger::DEBUG_UNQUALIFIED);
  TTCN_Logger::log_event_str(p_prompt);
  buffer.log();
  TTCN_Logger::end_event();
}

void loggers::log_to_hexa(const char *p_prompt, const OCTETSTRING &msg) {
  TTCN_Logger::begin_event(TTCN_Logger::DEBUG_UNQUALIFIED);
  TTCN_Logger::log_event_str(p_prompt);
  TTCN_Logger::log_event("Size: %d,\nMsg: ", msg.lengthof());

  for (int i = 0; i < msg.lengthof(); i++) {
    TTCN_Logger::log_event(" %02x", ((const unsigned char *)msg)[i]);
  }
  TTCN_Logger::log_event("\n");
  TTCN_Logger::end_event();
}

void loggers::log_to_hexa(const char *p_prompt, const unsigned char *msg, const size_t msg_size) {
  TTCN_Logger::begin_event(TTCN_Logger::DEBUG_UNQUALIFIED);
  TTCN_Logger::log_event_str(p_prompt);
  for (size_t i = 0; i < msg_size; i++) {
    TTCN_Logger::log_event(" %02x", *(msg + i));
  }
  TTCN_Logger::log_event("\n");
  TTCN_Logger::end_event();
}

void loggers::log_msg(const char *p_prompt, const Base_Type &p_type) {
  TTCN_Logger::begin_event(TTCN_Logger::DEBUG_UNQUALIFIED);
  TTCN_Logger::log_event_str(p_prompt);
  p_type.log();
  TTCN_Logger::end_event();
}

void loggers::log(const char *p_fmt, ...) {
  TTCN_Logger::begin_event(TTCN_Logger::DEBUG_UNQUALIFIED);
  va_list args;
  va_start(args, p_fmt);
  TTCN_Logger::log_event_va_list(p_fmt, args);
  va_end(args);
  TTCN_Logger::end_event();
}

void loggers::user_msg(const char *p_prompt, const Base_Type &p_type) {
  TTCN_Logger::begin_event(TTCN_Logger::USER_UNQUALIFIED);
  TTCN_Logger::log_event_str(p_prompt);
  p_type.log();
  TTCN_Logger::end_event();
}

void loggers::user(const char *p_fmt, ...) {
  TTCN_Logger::begin_event(TTCN_Logger::USER_UNQUALIFIED);
  va_list args;
  va_start(args, p_fmt);
  TTCN_Logger::log_event_va_list(p_fmt, args);
  va_end(args);
  TTCN_Logger::end_event();
}

void loggers::warning(const char *p_fmt, ...) {
  TTCN_Logger::begin_event(TTCN_Logger::WARNING_UNQUALIFIED);
  va_list args;
  va_start(args, p_fmt);
  TTCN_Logger::log_event_va_list(p_fmt, args);
  va_end(args);
  TTCN_Logger::end_event();
}

void loggers::warning_msg(const char *p_prompt, const Base_Type &p_type) {
  TTCN_Logger::begin_event(TTCN_Logger::WARNING_UNQUALIFIED);
  TTCN_Logger::log_event_str(p_prompt);
  p_type.log();
  TTCN_Logger::end_event();
}

void loggers::error(const char *p_fmt, ...) {
  va_list args;
  va_start(args, p_fmt);
  TTCN_error(p_fmt, args);
  va_end(args);
}

void loggers::set_start_time(std::string &p_time_key) { _times[p_time_key] = std::clock(); }

void loggers::set_stop_time(std::string &p_time_key, float &p_time) {
  std::map<std::string, std::clock_t>::iterator it = _times.find(p_time_key);
  if (it != loggers::_times.end()) {
    p_time = (std::clock() - _times[p_time_key]) * 1000.0 / CLOCKS_PER_SEC; // in milliseconds
    _times.erase(it);
    loggers::get_instance().log_time_exec("%s: Execution duration: %f ms", p_time_key.c_str(), p_time);
  }
}

void loggers::log_time_exec(const char *p_fmt, ...) {
  TTCN_Logger::begin_event(TTCN_Logger::EXECUTOR_RUNTIME);
  va_list args;
  va_start(args, p_fmt);
  TTCN_Logger::log_event_va_list(p_fmt, args);
  va_end(args);
  TTCN_Logger::end_event();
}
