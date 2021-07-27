#include <storedSettings.h>
#include "wsdebug.h"
#include "SPIFFS.h"
#include "dispenserManager.h"
#include "dispense.h"
#include "dispenseSerialization.h"
void DispenseManager::loadFromDisk() {
  dispenses.clear();
  auto loadedDispenses = storedSettings->get<JsonArray>("dispenses");
  if (loadedDispenses.isNull()) {
    debugI(logtags::dispense,"No dispense found on disk");
  }
  for (auto dispense : loadedDispenses) {
    if (!dispense.is<Dispense>()) {
      continue;
    }
    dispenses.push_back(dispense);
  }
  debugI(logtags::dispense,"Loaded %d dispenses from disk", dispenses.size());
}

void DispenseManager::writeToDisk() {
  storedSettings->runTransaction([&](JsonDocument& settings) {
    JsonArray loadedDispenses = settings.createNestedArray("dispenses");
    loadedDispenses.clear();
    for (int i = 0; i < dispenses.size(); i++) {
      loadedDispenses[i] = dispenses[i];
    }
  });
  debugI(logtags::dispense,"Wrote %d dispenses to disk", dispenses.size());
}

