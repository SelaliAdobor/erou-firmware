
#include "containerManager.h"
#include "wsdebug.h"
#include "json11.hpp"
#include "SPIFFS.h"

void ContainerManager::loadFromDisk() {
  static_assert(config::physical::containerCount < 999, "Serialization assumes a reasonable number of containers.");
  containerContents.clear();

  char indexString[5];
  for (int index = 0; index < config::physical::containerCount; index++) {
    itoa(index, indexString, 10);
    auto isEmpty = storedSettings->getBoolF(containerEmptyKey, indexString);
    if (!isEmpty.has_value() || isEmpty.value()) {
      debugE("Skipped container");
      continue;
    }
    std::string rootKey = fmt::sprintf(containerRootKey, indexString);
    Container container;
    container.deserializeFrom(storedSettings, rootKey.c_str());

    containerContents.insert(ContainerMap::value_type {index, container});
  }
}

void ContainerManager::writeToDisk() {
  char indexString[5];
  static_assert(config::physical::containerCount < 999, "Serialization assumes a reasonable number of containers.");

  storedSettings->runTransaction([&]() {
    for (int index = 0; index < config::physical::containerCount; index++) {
      itoa(index, indexString, 10);
      if (!containerContents.count(index)) {
        storedSettings->setBoolF(true, containerEmptyKey, indexString);
        continue;
      }
      storedSettings->setBoolF(false, containerEmptyKey, indexString);

      std::string rootKey = fmt::sprintf(containerRootKey, indexString);
      containerContents[index].serializeInto(storedSettings, rootKey.c_str());
    }
  });
}

