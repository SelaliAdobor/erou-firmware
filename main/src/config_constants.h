#ifndef CONFIG_CONSTANTS_H
#define CONFIG_CONSTANTS_H
#include <cstdint>
namespace config {

const int64_t startupDelay = 5000L;

namespace physical {
// Includes the skipped empty slot
const int containerCount = 8;
}
namespace network {
const char *const ntpServer = "pool.ntp.org";
const char *const hostname = "erou";
const int port = 80;
}

namespace debug {
const int bufferSize = 8192;
const int wsTaskStack = 8192;
const int wsTaskPriority = 7;
const char *const wsDebugEndpoint = "/debug";
}  // namespace debug

namespace serial {
const int64_t baud = 115200;
}

namespace blinkTask {
const int stackSize = 4026;
const int priority = 10;
constexpr bool enabled = true;
const int blinkInterval = 2000;
}  // namespace blink_task

namespace loopTask {
const int stackSize = 8192;
const int priority = 1;
const int core = 1;
}  // namespace blink_task

namespace stepper {
const int stepsPerRotation = 200;
const int microsteps = 256;
const int current = 1500;
const int acceleration = 200;
const int deceleration = 200;
}  // namespace stepper

namespace motion {
const int coastingDurationMs = 100;
const int maxHomingDurationMs = 1 * 60 * 1000;

// During homing, after initial approach, overshoot by this much then slowly
// return to home Needs to be an angle enough large enough to leave home
// position
const float homingCorrectionOvershoot = 25;

const float rpmHomingCorrection = 20;
const float rpmHomingTravel = 50;
const float rpmContainerTravelMin = 20;
const float rpmContainerTravelMax = 50;

const float angleBetweenContainers = 360.0 / physical::containerCount;
}  // namespace motion

namespace dispense {
const int taskStack = 8192;
const int taskPriority = 7;
const int maxContainersPerDispense = 10;
}
}  // namespace config


#endif // CONFIG_CONSTANTS_H