
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
  if (!hasContent[container]) {
    return {};
  }

  return containerContents[container];
}

void ContainerManager::setContainerContent(int container, const Container content) {
  if (container >= config::physical::containerCount) {
    debugE("Attempted to set out of bounds container %d", container);
    return;
  }
  containerContents[container] = content;
  hasContent[container] = true;
}

ContainerManager::ContainerManager() {
  std::fill_n(hasContent, config::physical::containerCount, 0);
}


void ContainerManager::setup() {
  loadFromDisk();
}
