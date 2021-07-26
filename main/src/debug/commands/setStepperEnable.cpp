#include <pins.h>
#include "commands.h"

void DebugCommands::setStepperEnable(const LongString& args) {
  uint8_t enabled = 0;
  if (sscanf(args.c_str(), "%" SCNu8, &enabled) != 1) {
    debugI("Failed to parse command.");
    return;
  }

  digitalWrite(pins::stepper::enable, !enabled);
  debugI("Updated stepper motor enabled state %d", enabled);
}
