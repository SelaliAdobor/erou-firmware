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
  ShortString id;
  LongString name;
  LongString description;
  int quantity;
};
