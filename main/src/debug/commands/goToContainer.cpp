#include "wsdebug.h"
#include "commands.h"

void DebugCommands::goToContainer(const LongString&  args) {
  int containerIndex = 0;

  if (sscanf(args.c_str(), "%d", &containerIndex) != 1) {
    debugI("Failed to parse command.");
    return;
  }

  debugI("Going to container %d for debug command", containerIndex);
  if (containerIndex >= config::physical::containerCount) {
    debugE(
        "Container index exceeded for goToContainer: %d out of %d possible",
        containerIndex, config::physical::containerCount);
    return;
  }
  motion->goToContainerAt(containerIndex);
  debugI("Went to container %d after debug command", containerIndex);
}
