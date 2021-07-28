#include "dispense.h"
#include "croncpp.h"

std::optional<double> Dispense::secondsUntil(tm startingTime) {
  auto cron = cron::make_cron(cronSchedule.c_str());
  if(!cron.has_value()){
    debugE(logtags::dispense, "Failed to parse Dispense cron string %s", cronSchedule.c_str())
    return {};
  }
  tm next = cron::cron_next(cron.value(), startingTime);
  time_t nowT = mktime(&startingTime);
  time_t nextT = mktime(&next);
  return difftime(nowT, nextT);
}

std::optional<double> Dispense::secondsUntil() {
  tm currentTime;
  if (!getLocalTime(&currentTime)) {
    debugE(logtags::dispense, "Unable to get local time");
    return {};
  }
  return secondsUntil(currentTime);
}
