#ifndef UpperTesterSremSsem_LAYER_H
#define UpperTesterSremSsem_LAYER_H

#include "UpperTesterSremSsemCodec.hh"
#include "t_layer.hh"

namespace LibItsSremSsem__TestSystem {
  class UpperTesterPort;
}

namespace LibItsSremSsem__TypesAndValues {
  class UtSremInitialize;
  class UtSremTrigger;
  class UtSremUpdate;
  class UtSremCancelation;
} // namespace LibItsSremSsem__TypesAndValues

class UpperTesterSremSsemLayer : public t_layer<LibItsSremSsem__TestSystem::UpperTesterPort> {
  params                   _params;
  UpperTesterSremSsemCodec _codec;

public:
  UpperTesterSremSsemLayer() : t_layer<LibItsSremSsem__TestSystem::UpperTesterPort>(), _params(), _codec(){};
  UpperTesterSremSsemLayer(const std::string &p_type, const std::string &param);
  virtual ~UpperTesterSremSsemLayer(){};

  void sendMsg(const LibItsSremSsem__TypesAndValues::UtSremInitialize &send_par, params &p_params);
  void sendMsg(const LibItsSremSsem__TypesAndValues::UtSremTrigger &send_par, params &p_params);
  void sendMsg(const LibItsSremSsem__TypesAndValues::UtSremUpdate &send_par, params &p_params);
  void sendMsg(const LibItsSremSsem__TypesAndValues::UtSremCancelation &send_par, params &p_params);

  virtual void send_data(OCTETSTRING &data, params &p_params);
  virtual void receive_data(OCTETSTRING &data, params &info);
};

#endif
