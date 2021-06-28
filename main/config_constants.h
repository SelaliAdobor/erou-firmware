#ifndef CONFIG_CONSTANTS_H
#define CONFIG_CONSTANTS_H

namespace config {

const int64_t startupDelay = 5000L;

namespace physical {
// Includes the skipped empty slot
const int containerCount = 8;
}
namespace network {
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
const int stackSize = 8192;
const int priority = 1;
constexpr bool enabled = true;
const int blinkInterval = 500;
}  // namespace blink_task

namespace loopTask {
const int stackSize = 8192;
const int priority = 1;
const int core = 1;
}  // namespace blink_task

namespace stepper {
const int stepsPerRotation = 200;
const int microsteps = 32;
const int acceleration = 300;
const int deceleration = 600;
}  // namespace stepper

namespace motion {
const int coastingDurationMs = 100;
const int maxHomingDurationMs = 1 * 60 * 1000;

// During homing, after initial approach, overshoot by this much then slowly
// return to home Needs to be an angle enough large enough to leave home
// position
const float homingCorrectionOvershoot = 45;

const float rpmHomingCorrection = 20;
const float rpmHomingTravel = 40;
const float rpmContainerTravelMin = 30;
const float rpmContainerTravelMax = 40;

const float angleBetweenContainers = 360.0 / physical::containerCount;
}  // namespace motion
}  // namespace config


#endif // CONFIG_CONSTANTS_H