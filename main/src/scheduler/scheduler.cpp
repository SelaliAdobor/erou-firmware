//
// Created by Selali Adobor on 7/9/21.
//

#include "scheduler.h"
#include "croncpp.h"
#include <chrono>
#include <wsdebug.h>
#include <SPIFFS.h>

double Scheduler::secondsUntilNextDispense() {
  double timeUntilSoonest = std::numeric_limits<double>::max();
  struct tm currentTime;

  if (!getLocalTime(&currentTime)) {
    debugE("Unable to get local time");
    return -1;
  }
  for (auto[_, container] : containerManager->getAllLoadedContainers()) {
    auto cron = cron::make_cron(container.cron);
    tm next = cron::cron_next(cron.value(), currentTime);
    time_t nowT = mktime(&currentTime);
    time_t nextT = mktime(&next);
    double diffSecs = difftime(nowT, nextT);
    if (diffSecs > 0 && diffSecs < timeUntilSoonest) {
      timeUntilSoonest = diffSecs;
    }
  }

  return timeUntilSoonest;
}
Scheduler::Scheduler(ContainerManager *container_manager) : containerManager(container_manager) {
  File timezoneFile = SPIFFS.open("/cfg/timezone", "w+");
  String savedTimezone = timezoneFile.readString();
  if (savedTimezone.length() > 0) {
    configTzTime(savedTimezone.c_str(), ntpServer);
  }
}
