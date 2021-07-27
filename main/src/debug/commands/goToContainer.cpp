#include "wsdebug.h"
#include "commands.h"

void DebugCommands::goToContainer(const LongString&  args) {
  int containerIndex = 0;

  if (sscanf(args.c_str(), "%d", &containerIndex) != 1) {
    debugI(logtags::debug, "Failed to parse command.");
    return;
  }

  debugI(logtags::debug, "Going to container %d for debug command", containerIndex);
  if (containerIndex >= config::physical::containerCount) {
    debugE(logtags::debug,
        "Container index exceeded for goToContainer: %d out of %d possible",
        containerIndex, config::physical::containerCount);
    return;
  }
  motion->goToContainerAt(containerIndex);
  debugI(logtags::debug, "Went to container %d after debug command", containerIndex);
}
