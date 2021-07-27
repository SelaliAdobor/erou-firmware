#include "containerManager.h"
#include "wsdebug.h"
#include "SPIFFS.h"
#include "containerSerialization.h"
void ContainerManager::loadFromDisk() {
  static_assert(config::physical::containerCount < 999, "Serialization assumes a reasonable number of containers.");
  containerContents.clear();
  auto containerArray = storedSettings->get<JsonArray>("containers");
  debugI(logtags::storage, "Found %d containers in settings", containerArray.size());
  for (int index = 0; index < config::physical::containerCount; index++) {
    JsonVariant container = containerArray.getElement(index);
    if(container.isNull() || container.isUndefined()){
      continue;
    }
    containerContents[index] = container;
  }
}

void ContainerManager::writeToDisk() {
  static_assert(config::physical::containerCount < 999, "Serialization assumes a reasonable number of containers.");

  storedSettings->runTransaction([&](JsonDocument& settings) {
    JsonArray containerArray = settings.createNestedArray("containers");
    for (int index = 0; index < config::physical::containerCount; index++) {
      if (!containerContents.count(index)) {
        containerArray[index] = nullptr;
        continue;
      }
      containerArray[index] = containerContents[index];
    }
  });
}

