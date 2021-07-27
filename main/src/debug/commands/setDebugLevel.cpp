#include "commands.h"
#include "wsdebug.h"
void DebugCommands::setDebugLevel(const LongString &args) {
  int newLevel = 0;
  if (sscanf(args.c_str(), "%d", &newLevel) != 1) {
    debugE(logtags::debug, "Failed to parse args %s", args.c_str());

  }
  debugInstance.loggingLevel = DebugLevel(newLevel);
  debugI(logtags::debug, "Setting log level to %d", static_cast<int>(debugInstance.loggingLevel));
}