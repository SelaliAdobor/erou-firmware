#pragma once
#include <motion.h>

#include <utility>
#include "wsdebug.h"
#include "debugCommand.h"

class DebugCommands {
  inline static Motion *motion;

  static void checkUptime(const ShortString &args);
  static void goToContainer(const ShortString &args);
  static void setStepperEnable(const ShortString &args);
  static void setDebugLevel(const ShortString &args);

 public:
  static constexpr DebugCommand registeredCommands[] = {
      {"ut", &DebugCommands::checkUptime},
      {"gtc", &DebugCommands::goToContainer},
      {"en", &DebugCommands::setStepperEnable},
      {"level", &DebugCommands::setDebugLevel}
  };

  static void setup(Motion *_motion) {
    DebugCommands::motion = _motion;
  }
};