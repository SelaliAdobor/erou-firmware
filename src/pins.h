#pragma once

namespace pins
{
    const int onboardLed = 2;

    namespace stepper
    {
        const int enable = 27;
        const int reset = 21;
        const int sleep = 25;

        const int direction = 33;
        const int step = 32;

        const int m0 = 23;
        const int m1 = 18;
        const int m2 = 5;

        const int fault = 5;

    }

    namespace sensors
    {

        const int homingHallEffect = 35;
    }

    inline void setup()
    {
        pinMode(pins::onboardLed, OUTPUT);
        pinMode(pins::stepper::enable, OUTPUT);
        pinMode(pins::stepper::reset, OUTPUT);
        pinMode(pins::stepper::sleep, OUTPUT);

        pinMode(pins::stepper::direction, OUTPUT);
        pinMode(pins::stepper::step, OUTPUT);

        pinMode(pins::stepper::m0, OUTPUT);
        pinMode(pins::stepper::m1, OUTPUT);
        pinMode(pins::stepper::m2, OUTPUT);

        pinMode(pins::stepper::fault, INPUT_PULLUP);
    }
}
