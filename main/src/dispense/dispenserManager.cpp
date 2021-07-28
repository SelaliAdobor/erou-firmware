#include "dispenserManager.h"
#include "croncpp.h"
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
  struct tm currentTime;

  if (!getLocalTime(&currentTime)) {
    debugE(logtags::dispense, "Unable to get local time");
    return {};
  }
  return getNextDispense(currentTime);
}

std::optional<Dispense> DispenseManager::getNextDispense(tm startingFrom) {
  if(dispenses.empty()){
    return {};
  }
  double timeUntilSoonest = std::numeric_limits<double>::max();
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
    delay(500);
    auto next = getNextDispense();
    if (!next.has_value() ) {
      continue;
    }
    struct tm currentTime;

    if (!getLocalTime(&currentTime)) {
      debugE(logtags::dispense, "Unable to get local time");
      continue;
    }
    auto timeUntilNext = (*next).secondsUntil(currentTime);
    if(!timeUntilNext.has_value()){
      debugE(logtags::dispense, "Dispense had invalid next time");
      continue;
    }
    auto msUntilNext = *timeUntilNext * 1000;
    debugE(logtags::dispense,
           "Waiting %f seconds to dispense %s",
           msUntilNext,
           (*next).name.c_str());
    vTaskDelayUntil(&lastDispenseTaskRun,
                    pdMS_TO_TICKS(msUntilNext));
    runDispense(*next);
  }
}

void DispenseManager::addDispense(Dispense dispense) {
  dispenses.push_back(dispense);
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
