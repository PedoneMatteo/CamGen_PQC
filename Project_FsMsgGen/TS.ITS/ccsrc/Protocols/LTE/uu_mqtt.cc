#include <thread>

#include "loggers.hh"

#include "uu_mqtt.hh"

#include "lte_udp_layer.hh"
#include "lte_tcp_layer.hh"

#include "base_time.hh"

MessageId uu_mqtt::message_id_counter = 0;

uu_mqtt::uu_mqtt() : _uu_transport("udp"), _protocol_name("MQTT"), _client_id("EtsiTS"), _state{idle}, _subscribed_topics(), _topics() {
  loggers::get_instance().log(">>> uu_mqtt::uu_mqtt");

}

uu_mqtt::~uu_mqtt() {
  loggers::get_instance().log(">>> uu_mqtt::~uu_mqtt");

  close();
}

void uu_mqtt::close() {
  loggers::get_instance().log(">>> uu_mqtt::close");

  if (_layer != nullptr) {
    loggers::get_instance().log("uu_mqtt::close: Send Disconnect command");
    delete _layer;
  }
}

int uu_mqtt::setup(const params &params) {
  loggers::get_instance().log(">>> uu_mqtt::setup");

  // Extract protocol name
  params::const_iterator it = params.find(params::mqtt_protocol);
  if (it != params.cend()) {
    _protocol_name.assign(it->second);
  }
  // Extract client identity
  it = params.find(params::mqtt_client_id);
  if (it != params.cend()) {
    _client_id.assign(it->second);
  }
  // Extract topics
  it = params.find(params::mqtt_topics);
  if (it != params.cend()) {
    if (it->second.compare("obu") == 0) { // Subscribe to /3gpp/v2x/obu published messages
      // OBUs publishes to RSUs topics
      _subscribed_topics.insert(std::pair<std::string, std::string>("141", "/3gpp/v2x/obu/beacon"));
      _subscribed_topics.insert(std::pair<std::string, std::string>("36", "/3gpp/v2x/obu/cam"));
      _subscribed_topics.insert(std::pair<std::string, std::string>("37", "/3gpp/v2x/obu/denm"));
      _subscribed_topics.insert(std::pair<std::string, std::string>("139", "/3gpp/v2x/obu/ivim"));
      _subscribed_topics.insert(std::pair<std::string, std::string>("138", "/3gpp/v2x/obu/mapem"));
      _subscribed_topics.insert(std::pair<std::string, std::string>("137", "/3gpp/v2x/obu/spatem"));
      _subscribed_topics.insert(std::pair<std::string, std::string>("140", "/3gpp/v2x/obu/sxem"));
      _subscribed_topics.insert(std::pair<std::string, std::string>("128", "/3gpp/v2x/obu/rtcmem"));
      // RSU publishes on OBUs topics
      _topics.insert(std::pair<std::string, std::string>("141", "/3gpp/v2x/rsu/beacon"));
      _topics.insert(std::pair<std::string, std::string>("36", "/3gpp/v2x/rsu/cam"));
      _topics.insert(std::pair<std::string, std::string>("37", "/3gpp/v2x/rsu/denm"));
      _topics.insert(std::pair<std::string, std::string>("139", "/3gpp/v2x/rsu/ivim"));
      _topics.insert(std::pair<std::string, std::string>("138", "/3gpp/v2x/rsu/mapem"));
      _topics.insert(std::pair<std::string, std::string>("137", "/3gpp/v2x/rsu/spatem"));
      _topics.insert(std::pair<std::string, std::string>("140", "/3gpp/v2x/rsu/sxem"));
      _topics.insert(std::pair<std::string, std::string>("128", "/3gpp/v2x/rsu/rtcmem"));
    } else { // Subscribe to /3gpp/v2x/rsu published messages
      // OBU subscribes to RSUs topics
      _subscribed_topics.insert(std::pair<std::string, std::string>("141", "/3gpp/v2x/rsu/beacon"));
      _subscribed_topics.insert(std::pair<std::string, std::string>("36", "/3gpp/v2x/rsu/cam"));
      _subscribed_topics.insert(std::pair<std::string, std::string>("37", "/3gpp/v2x/rsu/denm"));
      _subscribed_topics.insert(std::pair<std::string, std::string>("139", "/3gpp/v2x/rsu/ivim"));
      _subscribed_topics.insert(std::pair<std::string, std::string>("138", "/3gpp/v2x/rsu/mapem"));
      _subscribed_topics.insert(std::pair<std::string, std::string>("137", "/3gpp/v2x/rsu/spatem"));
      _subscribed_topics.insert(std::pair<std::string, std::string>("140", "/3gpp/v2x/rsu/sxem"));
      _subscribed_topics.insert(std::pair<std::string, std::string>("128", "/3gpp/v2x/rsu/rtcmem"));
      // RSU publishes on OBUs topics
      _topics.insert(std::pair<std::string, std::string>("141", "/3gpp/v2x/obu/beacon"));
      _topics.insert(std::pair<std::string, std::string>("36", "/3gpp/v2x/obu/cam"));
      _topics.insert(std::pair<std::string, std::string>("37", "/3gpp/v2x/obu/denm"));
      _topics.insert(std::pair<std::string, std::string>("139", "/3gpp/v2x/obu/ivim"));
      _topics.insert(std::pair<std::string, std::string>("138", "/3gpp/v2x/obu/mapem"));
      _topics.insert(std::pair<std::string, std::string>("137", "/3gpp/v2x/obu/spatem"));
      _topics.insert(std::pair<std::string, std::string>("140", "/3gpp/v2x/obu/sxem"));
      _topics.insert(std::pair<std::string, std::string>("128", "/3gpp/v2x/obu/rtcmem"));
    }
  } else {
    //topics=(3gpp/v2x/cam,3gpp/v2x/denm)
    // Decode the list of topics
  }
  // TODO: Protocol name and client_id

  // Transport layer to the broker
  it = params.find(params::uu_transport);
  if (it != params.cend()) {
    if (it->second.compare("tcp") == 0) {
      loggers::get_instance().log("uu_mqtt::setup: Create TCP layer");
      _uu_transport.assign("tcp");
      _layer = new lte_tcp_layer("TCP", params);
      ((lte_tcp_layer*)_layer)->incoming_packet_observer_attach(this);
    } else {
      loggers::get_instance().log("uu_mqtt::setup: Create UDP layer");
      _layer = new lte_udp_layer("UDP", params);
      ((lte_udp_layer*)_layer)->incoming_packet_observer_attach(this);
    }
  } else {
    loggers::get_instance().log("uu_mqtt::setup: Create UDP layer");
    _layer = new lte_udp_layer("UDP", params);
    ((lte_udp_layer*)_layer)->incoming_packet_observer_attach(this);
  }
  loggers::get_instance().log("uu_mqtt::setup: _layer %p", _layer);

  loggers::get_instance().log("uu_mqtt::setup: Send connect");
  send_connect_command();

  return 0;
}

int uu_mqtt::shutdown() {
  loggers::get_instance().log(">>> uu_mqtt::shutdown");

  if (_state != idle) {
    loggers::get_instance().log("uu_mqtt::shutdown: Send Disconnect command");
    send_disconnect_command();
    _state = idle;
  }
  // Don't delete transport layer

  return 0;
}

void uu_mqtt::send_data(OCTETSTRING &data, params &params) {
  loggers::get_instance().log(">>> uu_mqtt::send_data");

  send_publish_command(data, params);
}

void uu_mqtt::receive_data(OCTETSTRING &p_data, params &p_params) {
  loggers::get_instance().log(">>> uu_mqtt::receive_data: %d", _state);

  switch (_state) {
  case await_connect_ack:
    if (recv_connect_ack(p_data, p_params) == -1) {
      _state = error;
    } else {
      _state = await_subscribe_ack;
      loggers::get_instance().log("uu_mqtt::receive_data: Send Subscribe command");
      send_first_subscribe_command();
      loggers::get_instance().log("uu_mqtt::receive_data: Await subscribeAck");
    }
    break;
  case await_subscribe_ack:
    recv_subscribe_ack(p_data, p_params);
    send_next_subscribe_command();
    loggers::get_instance().log("uu_mqtt::receive_data: Send subscribe, state=%d", _state);
    break;
  case publishing: {
      OCTETSTRING os;
      if (recv_publish(p_data, p_params, os) == 1) {
        // Forward to notifier
        incoming_packet_notify(os, p_params);
      }
    } 
    break;
  case error:
    loggers::get_instance().log("uu_mqtt::receive_data: Error state, looping...");
    break;
  default:
    // Nothing to do
    break;
  } // End of 'switch'statement
}

void uu_mqtt::update(OCTETSTRING &p_data, params &p_params) {
  loggers::get_instance().log_msg(">>> uu_mqtt::update: ", p_data);

  receive_data(p_data, p_params);
}

void uu_mqtt::send_connect_command() {
  loggers::get_instance().log(">>> uu_mqtt::send_connect_command: %d", _state);

  if (_state != idle)  {
    loggers::get_instance().error("uu_mqtt::send_connect_command: Wrong state");
    return;
  }

  TTCN_Buffer data_message;
  data_message.put_string(int2oct(_protocol_name.length(), 2)); // Protocol name length
  data_message.put_string(char2oct(_protocol_name.c_str())); // Protocol name
  data_message.put_c(0x04); // Verion
  data_message.put_c(0x02); // Connect flag: Clean seesion flag set
  data_message.put_string(int2oct(30, 2)); // Keep alive
  data_message.put_string(int2oct(_client_id.length(), 2)); // Client ID length
  data_message.put_string(char2oct(_client_id.c_str())); // Client ID
  TTCN_Buffer data;
  data.put_c(0x10); // Connect command flag 0001 0000
  data.put_string(calculate_message_length(data_message.get_len())); // Message length
  data.put_string(OCTETSTRING(data_message.get_len(), data_message.get_data()));

  params param;
  OCTETSTRING os = OCTETSTRING(data.get_len(), data.get_data());
  loggers::get_instance().log_msg("uu_mqtt::send_connect_command: ", os);
  _state = await_connect_ack;
  _layer->send_data(os, param);
}

void uu_mqtt::send_first_subscribe_command() {
  loggers::get_instance().log(">>> uu_mqtt::send_first_subscribe_command: %d", _state);

  if (_state != await_subscribe_ack) {
    loggers::get_instance().warning("uu_mqtt::send_first_subscribe_command: wrong state: %d", _state);
    return;
  }

  _subscribed_topics_it = _subscribed_topics.begin();

  send_subscribe_command();
}

void uu_mqtt::send_next_subscribe_command() {
  loggers::get_instance().log(">>> uu_mqtt::send_next_subscribe_command: %d", _state);

  if (_state != await_subscribe_ack) {
    loggers::get_instance().warning("uu_mqtt::send_next_subscribe_command: wrong state: %d", _state);
    return;
  }

  _subscribed_topics_it++;
  if (_subscribed_topics_it == _subscribed_topics.end()) {
    loggers::get_instance().log("uu_mqtt::send_next_subscribe_command: No more entries, new state: %d", publishing);
    _state = publishing;
    return;
  }

  send_subscribe_command();

  return;
}

void uu_mqtt::send_subscribe_command() {
  loggers::get_instance().log(">>> uu_mqtt::send_subscribe_command: %s - %d", _subscribed_topics_it->second.c_str(), _state);

  TTCN_Buffer data_message;
  data_message.put_string(int2oct(++message_id_counter, 2)); // MessageId
  data_message.put_string(int2oct(_subscribed_topics_it->second.length(), 2)); // Topic length
  data_message.put_string(char2oct(_subscribed_topics_it->second.c_str())); // Topic name
  data_message.put_c(0x01); // Acknowleded deliver
  TTCN_Buffer data;
  data.put_c(0x82); // Subscribe command flag 1000 0010
  data.put_string(calculate_message_length(data_message.get_len())); // Message length
  data.put_string(OCTETSTRING(data_message.get_len(), data_message.get_data()));

  params param;
  OCTETSTRING os = OCTETSTRING(data.get_len(), data.get_data());
  loggers::get_instance().log_msg("uu_mqtt::send_subscribe_command: ", os);
  _layer->send_data(os, param);
}

void uu_mqtt::send_publish_command(const OCTETSTRING &p_data, const params &p_params) {
  loggers::get_instance().log(">>> uu_mqtt::send_publish_command: %d", _state);

  if (_state != publishing) {
    loggers::get_instance().warning("uu_mqtt::send_publish_command: wrong state: %d", _state);
    return;
  }

  params::const_iterator it = p_params.find(std::string("its_aid"));
  if (it == p_params.cend()) {
    loggers::get_instance().warning("uu_mqtt::send_publish_command: No its_aid found, discard it");
    return;
  }
  _topics_subscribe_it = _topics.find(it->second);
  if (_topics_subscribe_it == _topics.cend()) {
    loggers::get_instance().warning("uu_mqtt::send_publish_command: Invalid its_aid, discard it");
    return;
  }
  loggers::get_instance().log("uu_mqtt::send_publish_command: %s", _topics_subscribe_it->second.c_str());
 
  TTCN_Buffer data_message;
  data_message.put_string(int2oct(_topics_subscribe_it->second.length(), 2)); // Topic length
  data_message.put_string(char2oct(_topics_subscribe_it->second.c_str())); // Topic name
  data_message.put_string(int2oct(++message_id_counter, 2)); // MessageId
  data_message.put_string(p_data); // Message
  TTCN_Buffer data;
  data.put_c(0x32); // Publish command flag 0011 0010
  data.put_string(calculate_message_length(data_message.get_len())); // Message length
  data.put_string(OCTETSTRING(data_message.get_len(), data_message.get_data()));

  params param;
  OCTETSTRING os = OCTETSTRING(data.get_len(), data.get_data());
  loggers::get_instance().log_msg("uu_mqtt::send_publish_command: ", os);
  _layer->send_data(os, param);
}


void uu_mqtt::send_disconnect_command() {
  loggers::get_instance().log(">>> uu_mqtt::send_disconnect_command: %d", _state);

  if (_state == idle)  {
    loggers::get_instance().error("uu_mqtt::send_disconnect_command: Wrong state");
    return;
  }

  _state = idle;
}

int uu_mqtt::recv_connect_ack(OCTETSTRING &p_data, params &p_params) {
  loggers::get_instance().log(">>> uu_mqtt::recv_connect_ack: %d", _state);
  loggers::get_instance().log_msg(">>> uu_mqtt::recv_connect_ack", p_data);

  if ((p_data[0].get_octet() & 0xF0) != 0x20) {
    // Expecting ConnectACK
    loggers::get_instance().log("uu_mqtt::recv_connect_ack: Expecting ConnectACK, got %02x", p_data[0].get_octet());
    return -1;
  }
  if (p_data[1].get_octet() != 2) {
     // Expecting length set to 2
    loggers::get_instance().log("uu_mqtt::recv_connect_ack: Expecting length set to 2, got %02x", p_data[1].get_octet());
    return -1;
 
  }
  if (p_data[3].get_octet() != 0) {
    // Expecting ConnectionAccepted
    loggers::get_instance().log("uu_mqtt::recv_connect_ack: Expecting ConnectionAccepted, got %02x", p_data[3].get_octet());
    return -1;
  }

  return 0;
}

int uu_mqtt::recv_subscribe_ack(OCTETSTRING &p_data, params &p_params) {
  loggers::get_instance().log(">>> uu_mqtt::recv_subscribe_ack: %d", _state);
  loggers::get_instance().log_msg(">>> uu_mqtt::recv_subscribe_ack", p_data);

  if ((p_data[0].get_octet() & 0xF0) != 0x90) {
    // Expecting SubscribeACK
    loggers::get_instance().log("uu_mqtt::recv_subscribe_ack: Expecting SubscribeACK, got %02x", p_data[0].get_octet());
    return -1;
  }
  if (p_data[1].get_octet() != 3) {
     // Expecting length set to 3
    loggers::get_instance().log("uu_mqtt::recv_subscribe_ack: Expecting length set to 3, got %02x", p_data[1].get_octet());
    return -1;
 
  }
  
  return 0;
}

int uu_mqtt::recv_publish(OCTETSTRING &p_data, params &p_params, OCTETSTRING& p_message) {
  loggers::get_instance().log(">>> uu_mqtt::recv_publish: %d", _state);
  loggers::get_instance().log_msg(">>> uu_mqtt::recv_publish", p_data);

  if ((p_data[0].get_octet() & 0xF0) == 0x40) { // Publish ACK
    // Nothing to do here
  } else if ((p_data[0].get_octet() & 0xF0) == 0x30) { // Publish, received message
    unsigned int counter = 1;
    // Decode message length
    unsigned int l = extract_message_length(p_data, &counter);
    if (l < 0) {
      loggers::get_instance().log("uu_mqtt::recv_publish: Failed to extract message length");
      return -1;
    }
    loggers::get_instance().log("uu_mqtt::recv_publish: message length: %d - counter: %d", l, counter);
    // Extract Topic length
    unsigned int topic_len = p_data[counter++].get_octet() << 8 | p_data[counter++].get_octet();
    l -= 2;
    loggers::get_instance().log("uu_mqtt::recv_publish: topic_len: %d - counter: %d - l: %d", topic_len, counter, l);
    // Extract Topic
    std::string topic(static_cast<const char*>(oct2char(OCTETSTRING(topic_len, counter + static_cast<const unsigned char*>(p_data)))));
    loggers::get_instance().log("uu_mqtt::recv_publish: topic: %s", topic.c_str());
    counter += topic_len;
    l -= topic_len;
    loggers::get_instance().log("uu_mqtt::recv_publish: counter: %d - l: %d", counter, l);
    // Set params
    p_params.insert(std::pair<std::string, std::string>("topic", topic));
    // Extract MessageId
    counter += 2;
    l -= 2;
    // Extract message
    p_message = OCTETSTRING(l, counter + static_cast<const unsigned char*>(p_data));
    loggers::get_instance().log_msg("uu_mqtt::recv_publish: Received data: ", p_message);

    return 1;
  } else {
    // Expecting Publish/PublishACK
    loggers::get_instance().log("uu_mqtt::recv_publish: Expecting Publish/PublishACK, got %02x", p_data[0].get_octet());
    return -1;
  }
  
  return 0;
}

OCTETSTRING uu_mqtt::calculate_message_length(const int p_payload_len) {
  loggers::get_instance().log(">>> uu_mqtt::calculate_message_length: %d ", p_payload_len);

  int x = p_payload_len;
  int encoded_bytes = 0;
  int output = 0;
  do {
    encoded_bytes = x % 128;
    x = x / 128;
    // if there are more data to encode, set the top bit of this byte
    if (x > 0) {
      encoded_bytes |= 128;
    }
    output = output << 8 | encoded_bytes;
  } while (x > 0);
  OCTETSTRING os;
  if (output < 128) {
    os = int2oct(output, 1);
  } else {
    os = int2oct(output, 2);
  }

  loggers::get_instance().log_msg("<<< uu_mqtt::calculate_message_length: ", os);
  return os;
}

int uu_mqtt::extract_message_length(OCTETSTRING &p_data, unsigned int* p_counter) {
  loggers::get_instance().log(">>> uu_mqtt::extract_message_length");

  unsigned char c;
  int multiplier = 1;
  int len = 0;
  int value = 0;
  do {
    //loggers::get_instance().log("uu_mqtt::extract_message_length: ================================");
    if (++len > 4) {
      loggers::get_instance().warning("uu_mqtt::extract_message_length: Lenght exeeding 4 bytes");
      return -1;
    }
    if (p_data.lengthof() == *p_counter) {
      loggers::get_instance().warning("uu_mqtt::extract_message_length: Reach the end of the buffer");
      return -1;
    }
    c = p_data[*p_counter].get_octet();
    *p_counter += 1;
    //loggers::get_instance().log("uu_mqtt::extract_message_length: c=%02x", c);
    value += (c & 127) * multiplier;
    loggers::get_instance().log("uu_mqtt::extract_message_length: value=%d", value);
    multiplier *= 128;
    //loggers::get_instance().log("uu_mqtt::extract_message_length: multiplier=%d", multiplier);
    //loggers::get_instance().log("uu_mqtt::extract_message_length: c & 128=%02x", c & 128);
  } while ((c & 128) != 0);

  loggers::get_instance().log("<<< uu_mqtt::extract_message_length: %d", value);
  return value; 
}