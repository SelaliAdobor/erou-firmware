#pragma once

#include <string>
#include <vector>
#include <storedSettings.h>
#include <etl/vector.h>
#include <etl_types.h>
using ContainerIdList = etl::vector<ShortString, config::dispense::maxContainersPerDispense>;

class Dispense {

 public:
  ShortString id;
  ShortString name;
  ShortString cronSchedule;
  ContainerIdList containerIds;
  void serializeInto(StoredSettings* store, const char *rootKey) const;
  void deserializeFrom(StoredSettings* store, const char *rootKey);
  double secondsUntil(tm startingTime);
};