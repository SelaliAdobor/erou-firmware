#pragma once

namespace config
{
    //Includes the skipped empty slot
    const int containerCount = 8;

    const bool runningBlink = true;
    const int runningBlinkInterval = 500;


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

        const float rpmHomingCorrection = 1;
        const float rpmHomingTravel = 35;
        const float rpmContainerTravelMin = 10;
        const float rpmContainerTravelMax = 35;

        const float angleBetweenContainers = 360.0 / containerCount;
    }
}