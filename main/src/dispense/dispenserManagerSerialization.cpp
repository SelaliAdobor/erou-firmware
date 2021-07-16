#include <storedSettings.h>
#include "containerManager.h"
#include "wsdebug.h"
#include "json11.hpp"
#include "SPIFFS.h"
#include "dispenserManager.h"
#include "dispense.h"

void DispenseManager::loadFromDisk() {
  dispenses.clear();

  if (auto dispenseCount = storedSettings->getInt("dispenses/count")) {
    dispenses = DispenseList(*dispenseCount);
    for (int index = 0; index < config::physical::containerCount; index++) {
      std::string rootKey = fmt::sprintf("dispenses/%d/", index);
      Dispense dispense;
      dispense.deserializeFrom(storedSettings, rootKey.c_str());
      dispenses[index] = dispense;
    }
    debugI("Loaded %d dispenses from disk", dispenses.size());
  }

}

void DispenseManager::writeToDisk() {
  storedSettings->setInt("dispenses/count", static_cast<int>(dispenses.size()));
  for (int i = 0; i < dispenses.size(); i++) {
    std::string rootKey = fmt::sprintf("dispenses/%d/", i);
    dispenses[i].serializeInto(storedSettings, rootKey.c_str());
  }
  debugI("Wrote %d dispenses to disk", dispenses.size());
}

