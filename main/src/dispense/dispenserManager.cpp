#include "dispenserManager.h"
#include <chrono>
#include <wsdebug.h>
#include <SPIFFS.h>
#include "free_rtos_util.h"

void DispenseManager::setup() {
  loadFromDisk();
  xTaskCreate(toFreeRtosTask(DispenseManager, runDispenseTask),
              "Dispense task",
              config::debug::wsTaskStack,
              this,
              config::debug::wsTaskPriority,
              nullptr
  );
}

std::optional<Dispense> DispenseManager::getNextDispense() {
  return getNextDispense(time(nullptr));
}

std::optional<Dispense> DispenseManager::getNextDispense(time_t startingFrom) {
  if (dispenses.empty()) {
    return {};
  }
  size_t timeUntilSoonest = std::numeric_limits<size_t>::max();
  std::optional<Dispense> soonest = {};

  for (Dispense dispense : dispenses) {
    auto secondsUntilNext = dispense.secondsUntil(startingFrom);
    if (secondsUntilNext.has_value() && secondsUntilNext >= 0 && secondsUntilNext < timeUntilSoonest) {
      timeUntilSoonest = *secondsUntilNext;
      soonest = dispense;
    }
  }

  return soonest;
}

void DispenseManager::runDispense(const Dispense &dispense) {
  for (const auto &containerId : dispense.containerIds) {
    auto foundContainer = containerManager->getById(containerId);
    if (!foundContainer.has_value()) {
      debugE(logtags::dispense, "Attempted to dispense invalid container %s", containerId.c_str());
      continue;
    }
    debugE(logtags::dispense, "Dispensing: %s", containerId.c_str());
    auto[index, container] = foundContainer.value();
    motion->goToContainerAt(index);
    delay(10000); //Todo wait for input
    debugE(logtags::dispense, "Dispensed: %s", containerId.c_str());
  }
}

void DispenseManager::runDispenseTask() {
  for (;;) {
    reset:
    invalidateDispense = false;
    struct tm checkTime;
    if (!getLocalTime(&checkTime)) {
      debugE(logtags::dispense, "Unable to get local time");
      delay(10000);
      continue;
    }

    auto next = getNextDispense();
    if (!next.has_value()) {
      debugV(logtags::dispense, "No dispense found");
      delay(10000);
      continue;
    }

    time_t startTime;
    time(&startTime);

    std::optional<time_t> timeUntilNext = next->secondsUntil(startTime);
    if (!timeUntilNext.has_value()) {
      debugV(logtags::dispense, "Time until dispense invalid");
      delay(10000);
      continue;
    }
    time_t timeNextStarts = startTime + *timeUntilNext;
    time_t timeRemaining;
    do {
      if (invalidateDispense) {
        goto reset;
      }
      timeRemaining = timeNextStarts - time(nullptr);
      debugV(logtags::dispense, "Time until dispense %ld:", timeRemaining);

      delay(std::min(timeRemaining, time_t(10)) * 1000);
    } while (timeRemaining > 0);

    if (!invalidateDispense) {
      debugI(logtags::dispense, "Dispensing!");
      runDispense(*next);
    }
  }
}

void DispenseManager::addDispense(Dispense dispense) {
  dispenses.push_back(dispense);
  invalidateDispense = true;
  writeToDisk();
}

std::optional<Dispense> DispenseManager::getDispenseById(const ShortString &id) {
  auto dispense = std::find_if(dispenses.begin(),
                               dispenses.end(),
                               [id](const Dispense &dispense) -> bool {
                                 return dispense.id == id;
                               });

  return dispense != dispenses.end() ? std::make_optional<Dispense>()
                                     : std::optional<Dispense>(*dispense);
}

