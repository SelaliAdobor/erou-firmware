#pragma once

namespace config {

const long startupDelay = 5000L;
// Includes the skipped empty slot
const int containerCount = 8;

const char *const hostname = "erou";
namespace debug {
const int bufferSize = 8192;
const int wsDebugPort = 81;
const int wsTaskStack = 8192;
const int wsTaskPriority = 7;
const int maxMessageCount = 1000;
const int maxMessageTimeoutMicros = 1000;
}  // namespace debug
namespace serial {
const long serialBaud = 115200;
}

namespace blinkTask {
const int stackSize = 8192;
const int priority = 1;
const bool enabled = true;
const int blinkInterval = 500;
}  // namespace blinkTask

namespace loopTask {
const int stackSize = 8192;
const int priority = 1;
const int core = 1;
}  // namespace loopTask

namespace stepper {
const int stepsPerRotation = 200;
const int microsteps = 32;
const int acceleration = 300;
const int deceleration = 600;
}  // namespace stepper

namespace motion {
const int coastingDurationMs = 100;
const int maxHomingDurationMs = 1 * 60 * 1000;

// During homing, after intial approach, overshoot by this much then slowly
// return to home Needs to be an angle enough large enough to leave home
// position
const float homingCorrectionOvershoot = 45;

const float rpmHomingCorrection = 20;
const float rpmHomingTravel = 40;
const float rpmContainerTravelMin = 30;
const float rpmContainerTravelMax = 40;

const float angleBetweenContainers = 360.0 / containerCount;
}  // namespace motion
}  // namespace config