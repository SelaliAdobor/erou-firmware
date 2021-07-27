
#include <config_constants.h>
#include <optional>
#include "containerManager.h"
#include "Arduino.h"
#include "wsdebug.h"
#include "container.h"

std::optional<Container> ContainerManager::getContainerContent(const int container) {
  if (container >= config::physical::containerCount) {
    debugE(logtags::dispense,"Attempted to retrieve out of bounds container %d", container);
    return {};
  }
  if (!containerContents.count(container)) {
    return {};
  }

  return containerContents[container];
}

void ContainerManager::setContainerContent(int container, const Container &content) {
  if (container >= config::physical::containerCount) {
    debugE(logtags::dispense,"Attempted to set out of bounds container %d", container);
    return;
  }
  containerContents[container] = content;
  writeToDisk();
}

ContainerManager::ContainerManager(StoredSettings *stored_settings) : storedSettings(stored_settings) {}

void ContainerManager::setup() {
  loadFromDisk();
}
ContainerMap ContainerManager::getAllLoadedContainers() {
  return containerContents;
}
std::optional<etl::pair<int, Container>> ContainerManager::getById(const ShortString& id) {
  for (auto[index, container] : containerContents) {
    if (container.id.compare(id.data()) == 0) {
      return etl::pair<int, Container>({index, container});
    }
  }
  return {};
}
