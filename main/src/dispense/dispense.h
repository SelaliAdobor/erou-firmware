#pragma once

#include <string>
#include <vector>
#include <storedSettings.h>
#include <etl/vector.h>

static constexpr size_t maxContainersPerDispense = 10;
using ContainerIdList = etl::vector<std::string, maxContainersPerDispense>;

class Dispense {

 public:
  std::string id;
  std::string name;
  std::string cronSchedule;
  void serializeInto(StoredSettings* store, const char *rootKey) const;
  void deserializeFrom(StoredSettings* store, const char *rootKey);
  double secondsUntil(tm startingTime);
  ContainerIdList containerIds;
};