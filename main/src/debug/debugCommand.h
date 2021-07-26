#pragma once
#include "etl_types.h"

struct DebugCommand {
  etl::string_view name;
  void (*run)(const LongString&);
};