/*!
 * \file      registration.hh
 * \brief     Header file for the control port registration functionality.
 * \author    ETSI STF525
 * \copyright ETSI Copyright Notification
 *            No part may be reproduced except as authorized by written permission.
 *            The copyright and the foregoing restriction extend to reproduction in all media.
 *            All rights reserved.
 * \version   0.1
 */
#pragma once

#include <map>
#include <string>

/**
 * \class registration
 * \brief This class provides a way to register items and its object reference
 *        The registered items are accessible from any object (singleton pattern)
 * \remark There is one limitation: each item is uniquely defined in the process
 */
template <typename TItem> class registration { // TODO Refine the naming, derive from std::map<std::string, TItem*>
  std::map<std::string, TItem *> _items;

  static registration<TItem> *_instance;

private:
  registration() : _items(){}; // can not be created manually
public:
  static registration<TItem> &get_instance();

  virtual ~registration() {
    if (_instance != nullptr)
      delete _instance;
  };

public:
  void   add_item(const std::string &type, TItem *f);
  TItem *get_item(const std::string &type);
}; // End of class registration

template <typename TItem> registration<TItem> *registration<TItem>::_instance = nullptr;

// static functions
template <typename TItem> registration<TItem> &registration<TItem>::get_instance() {
  return (_instance != nullptr) ? *_instance : *(_instance = new registration());
}

template <typename TItem> void registration<TItem>::add_item(const std::string &type, TItem *f) { _items[type] = f; }

template <typename TItem> TItem *registration<TItem>::get_item(const std::string &type) {
  typename std::map<std::string, TItem *>::const_iterator it = _items.find(type);
  if (it == _items.cend()) {
    return nullptr;
  }

  return it->second;
} // End of class registration
