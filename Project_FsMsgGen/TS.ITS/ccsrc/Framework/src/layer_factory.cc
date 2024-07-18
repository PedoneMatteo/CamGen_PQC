#include <regex>
#include <stdexcept>

#include "layer_stack_builder.hh"

#include "loggers.hh"

layer_stack_builder *layer_stack_builder::_instance = NULL;

// static functions
layer_stack_builder *layer_stack_builder::get_instance() { return _instance ? _instance : _instance = new layer_stack_builder(); }

void layer_stack_builder::register_layer_factory(const std::string &p_type, layer_factory *p_layer_factory) {
  layer_stack_builder::get_instance()->_register_layer_factory(p_type, p_layer_factory);
}

// member functions
layer_stack_builder::layer_stack_builder() {}

void layer_stack_builder::_register_layer_factory(const std::string &p_type, layer_factory *p_layer_factory) { _layer_factories[p_type] = p_layer_factory; }

layer *layer_stack_builder::create_layer_stack(const char *p_layer_stack_description) {
  loggers::get_instance().log(">>> layer_stack_builder::create_layer_stack: %s", p_layer_stack_description);

  layer *entry = NULL; // Initial layer (the first declared)
  layer *up    = NULL; // Upper layer
  // Parse the layer description
  try {
    std::regex           rgx("(\\w+)(\\((.*?)\\))?(\\/|$)");
    std::string          str = p_layer_stack_description;
    std::sregex_iterator begin(str.cbegin(), str.cend(), rgx);
    std::sregex_iterator end = std::sregex_iterator();
    for (std::sregex_iterator it = begin; it != end; ++it) {
      std::smatch m = *it;
      loggers::get_instance().log("layer_stack_builder::create_layer_stack: %d - %s - %s - %s - %s", m.size(), m[0].str().c_str(), m[1].str().c_str(),
                                  m[2].str().c_str(), m[3].str().c_str());
      LayerFactoryMap::iterator i = _layer_factories.find(m[1].str());
      if (i == _layer_factories.end()) {
        loggers::get_instance().error("layer_stack_builder::create_layer_stack: %s: Unknown layer type", m[1].str().c_str());
      }
      loggers::get_instance().log("layer_stack_builder::create_layer_stack: Create layer %s, %s", m[1].str().c_str(), m[3].str().c_str());
      layer *l = i->second->create_layer(m[1].str(), m[3].str());
      if (NULL == l) {
        loggers::get_instance().error("layer_stack_builder::create_layer_stack: %s: Layer creation error", m[1].str().c_str());
      }

      loggers::get_instance().log("layer_stack_builder::create_layer_stack: Setup layers for %s", l->to_string().c_str());
      l->add_upper_layer(up);
      if (entry == NULL) { // Set the first declared layer
        entry = l;
      }
      up = l; // Build the linked list of layers
    }         // End of 'for' statement
  } catch (const std::logic_error &e) {
    if (up) { // FIXME To be reviewed
      up->delete_layer();
      up = NULL;
    }
  }
  return entry;
}
