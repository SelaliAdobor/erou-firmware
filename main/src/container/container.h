#pragma once

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <cstdint>
#include <ESP_FlexyStepper.h>
#include <functional>
#include <TMCStepper.h>
#include <memory>
#include <utility>
#include <storedSettings.h>
#include "json11.hpp"
#include "etl_types.h"

class Container {
 public:
  static const int currentVersion = 0x01;
  static const char separator = 0x1E;
  static const uint32_t maxNameLength = shortStringLength;
  static const uint32_t maxDescriptionLength = longStringLength;
  static const uint32_t maxCronLength = shortStringLength;

  ShortString id;
  ShortString name;
  LongString description;
  int quantity;
  void serializeInto(StoredSettings* store, const char* rootKey) const ;
  void deserializeFrom(StoredSettings* store,const char* rootKey) ;
};
