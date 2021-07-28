#include "dispense.h"
#include "ccronexpr.h"

std::optional<time_t> Dispense::secondsUntil(time_t startingTime) {
  cron_expr expr;
  const char* err = nullptr;
  memset(&expr, 0, sizeof(expr));
  cron_parse_expr(cronSchedule.c_str(), &expr, &err);

  if(err != nullptr){
    debugE(logtags::dispense, "Failed to parse Dispense cron string %s: %s", cronSchedule.c_str(), err)
    return {};
  }

  time_t nextT = cron_next(&expr, startingTime);
  if(nextT < 0){
    debugE(logtags::dispense, "Parsed cron string couldn't be converted to time %s", cronSchedule.c_str())
    return {};
  }
  auto timeUntil = difftime(nextT, startingTime);
  if(timeUntil < 0){
    debugE(logtags::dispense, "Parsed cron string resulted in past time %s", cronSchedule.c_str())
    return {};
  }
  return timeUntil;
}

std::optional<time_t> Dispense::secondsUntil() {
  return secondsUntil(time(nullptr));
}
