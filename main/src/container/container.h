#pragma once

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "binn.h"
#include <cstdint>
#include <ESP_FlexyStepper.h>
#include <functional>
#include <TMCStepper.h>
#include <memory>
#include <utility>
#include <FlexyStepper.h>
#include "ArduinoJson.h"
class Container {
 public:
  static const uint32_t currentVersion = 0x01;
  static const char separator = 0x1E;
  static const uint32_t maxNameLength = 128;
  static const uint32_t maxDescriptionLength = 512;
  static const uint32_t maxCronLength = 20;

  std::string name;
  std::string description;
  uint32_t quantity;
  std::string cron;
  void serializeInto(JsonObject& destination) const;
  void deserializeFrom(JsonObject source);
};
