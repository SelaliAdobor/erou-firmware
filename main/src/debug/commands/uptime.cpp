
#include "wsdebug.h"
#include "commands.h"

void DebugCommands::checkUptime(const LongString &) {
  debugI(logtags::debug, "%d seconds since last reboot", int (esp_timer_get_time() / 1000000));
}