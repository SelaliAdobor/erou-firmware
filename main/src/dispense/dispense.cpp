#include "dispense.h"
#include "croncpp.h"

double Dispense::secondsUntil(tm startingTime) {
  auto cron = cron::make_cron(cronSchedule.c_str());
  tm next = cron::cron_next(cron.value(), startingTime);
  time_t nowT = mktime(&startingTime);
  time_t nextT = mktime(&next);
  return difftime(nowT, nextT);
}
