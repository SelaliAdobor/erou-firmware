#pragma once

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <cstdint>
#include <ESP_FlexyStepper.h>
#include <functional>
#include <TMCStepper.h>
#include <memory>
#include <utility>
#include "json11.hpp"
class Container {
 public:
  static const int currentVersion = 0x01;
  static const char separator = 0x1E;
  static const uint32_t maxNameLength = 128;
  static const uint32_t maxDescriptionLength = 512;
  static const uint32_t maxCronLength = 20;

  std::string name;
  std::string description;
  int quantity;
  std::string cron;
  void serializeInto(json11::Json::object& destination) const;
  void deserializeFrom(json11::Json::object source);
};
