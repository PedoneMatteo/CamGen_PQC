#ifndef UpperTesterRtcmem_LAYER_H
#define UpperTesterRtcmem_LAYER_H

#include "UpperTesterRtcmemCodec.hh"
#include "t_layer.hh"

namespace LibItsRtcmem__TestSystem {
  class UpperTesterPort;
}

namespace LibItsRtcmem__TypesAndValues {
  class UtRtcmemInitialize;
  class UtRtcmemTrigger;
  class UtRtcmemUpdate;
  class UtRtcmemTermination;
} // namespace LibItsRtcmem__TypesAndValues

class UpperTesterRtcmemLayer : public t_layer<LibItsRtcmem__TestSystem::UpperTesterPort> {
  params                 _params;
  UpperTesterRtcmemCodec _codec;

public:
  UpperTesterRtcmemLayer() : t_layer<LibItsRtcmem__TestSystem::UpperTesterPort>(), _params(), _codec(){};
  UpperTesterRtcmemLayer(const std::string &p_type, const std::string &param);
  virtual ~UpperTesterRtcmemLayer(){};

  void sendMsg(const LibItsRtcmem__TypesAndValues::UtRtcmemInitialize &send_par, params &p_params);
  void sendMsg(const LibItsRtcmem__TypesAndValues::UtRtcmemTrigger &send_par, params &p_params);
  void sendMsg(const LibItsRtcmem__TypesAndValues::UtRtcmemUpdate &send_par, params &p_params);
  void sendMsg(const LibItsRtcmem__TypesAndValues::UtRtcmemTermination &send_par, params &p_params);

  virtual void send_data(OCTETSTRING &data, params &p_params);
  virtual void receive_data(OCTETSTRING &data, params &info);
};

#endif
