
#include <config_constants.h>
#include <optional>
#include "containerManager.h"
#include "Arduino.h"
#include "wsdebug.h"
#include "container.h"

std::optional<Container> ContainerManager::getContainerContent(const int container) {
  if (container >= config::physical::containerCount) {
    debugE("Attempted to retrieve out of bounds container %d", container);
    return {};
  }
  if (!containerContents.count(container)) {
    return {};
  }

  return containerContents[container];
}

void ContainerManager::setContainerContent(int container, const Container content) {
  if (container >= config::physical::containerCount) {
    debugE("Attempted to set out of bounds container %d", container);
    return;
  }
  containerContents.insert(ContainerMap::value_type(container, content));
  writeToDisk();
}

ContainerManager::ContainerManager() {
}


void ContainerManager::setup() {
  loadFromDisk();
}
ContainerMap ContainerManager::getAllLoadedContainers() {
  return containerContents;
}
