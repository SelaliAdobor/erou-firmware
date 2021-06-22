#pragma once

namespace config
{

    const long startupDelay = 5000L;
    //Includes the skipped empty slot
    const int containerCount = 8;

    const char *const hostname = "erou";
    namespace debug
    {
        const int bufferSize = 8192;
        const int wsDebugPort = 81;
        const int wsTaskStack = 8192;
        const int wsTaskPriority = 7;
        const int maxMessageCount = 1000;
        const int maxMessageTimeoutMicros = 1000;
    }
    namespace serial
    {
        const long serialBaud = 115200;
    }

    namespace blinkTask
    {
        const int stackSize = 8192;
        const int priority = 1;
        const bool enabled = true;
        const int blinkInterval = 500;
    }

    namespace loopTask
    {
        const int stackSize = 8192;
        const int priority = 1;
        const int core = 1;
    }

    namespace stepper
    {
        const int stepsPerRotation = 200;
        const int microsteps = 32;
        const int acceleration = 1000;
        const int deceleration = 1000;
    }

    namespace motion
    {
        const int coastingDurationMs = 200;
        const int maxHomingDurationMs = 1 * 60 * 1000;

        const float rpmHomingCorrection = 10;
        const float rpmHomingTravel = 35;
        const float rpmContainerTravelMin = 20;
        const float rpmContainerTravelMax = 35;

        const float angleBetweenContainers = 360.0 / containerCount;
    }
}