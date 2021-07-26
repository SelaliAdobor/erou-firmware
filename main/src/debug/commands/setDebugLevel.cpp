#include "commands.h"

void DebugCommands::setDebugLevel(const LongString &args) {
  int newLevel = 0;
  if (sscanf(args.c_str(), "%d", &newLevel) != 1) {
    debugE("Failed to parse args %s", args.c_str());

  }
  debugInstance.loggingLevel = DebugLevel(newLevel);
  debugI("Setting log level to %d", debugInstance.loggingLevel);
}