#include "loggers.hh"

std::unique_ptr<loggers> loggers::_instance = static_cast<std::unique_ptr<loggers>>(new loggers);

