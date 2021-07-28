#pragma once

#include <string>
#include <vector>
#include <storedSettings.h>
#include <etl/vector.h>
#include <etl_types.h>
using ContainerIdList = etl::vector<std::string, config::dispense::maxContainersPerDispense>;

class Dispense {

 public:
  ShortString id;
  ShortString name;
  ShortString cronSchedule;
  ContainerIdList containerIds;
  std::optional<double> secondsUntil(tm startingTime);
  std::optional<double> secondsUntil();
};