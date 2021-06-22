#include <Arduino.h>
#include "DRV8825.h"
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <limits>
#include <functional>
#include "pins.h"
#include "config.h"
#include "motion.h"
#include "debug.h"

Motion::Motion(BasicStepperDriver step) : stepper(step)
{
}

void setupStepperPins()
{
    digitalWrite(pins::stepper::enable, LOW);
    digitalWrite(pins::stepper::sleep, HIGH);
    digitalWrite(pins::stepper::reset, HIGH);
}

void Motion::setup()
{
    debugV("motion setup setting up stepper pins");
    setupStepperPins();

    debugV("motion setup setting up homing");
    setupHoming();

    delay(1000);

    stepper.setEnableActiveState(LOW);
    stepper.begin(33, config::stepper::microsteps);
    stepper.disable();
    stepper.enable();

    digitalWrite(pins::stepper::m0, HIGH);
    digitalWrite(pins::stepper::m1, HIGH);
    digitalWrite(pins::stepper::m2, HIGH);
    debugV("motion setup completed");
}

void Motion::setSpeedControl(bool enabled)
{
    if (enabled)
    {
        stepper.setSpeedProfile(stepper.LINEAR_SPEED, config::stepper::acceleration, config::stepper::deceleration);
    }
    else
    {
        //Acceleration is ignored for constant speed
        stepper.setSpeedProfile(stepper.CONSTANT_SPEED, 9999, 9999);
    }

    debugV("motion speed control changed %d", enabled);
}

void Motion::goToContainerAt(const int index)
{
    debugV("motion going to container, index %d", index);

    debugV("motion going to container starting homing");
    goToHome(false);
    debugV("motion going to container homed");
    float scaledRpm = map(index, 0, config::containerCount - 1, config::motion::rpmContainerTravelMin, config::motion::rpmContainerTravelMax);
    float rotation = index * config::motion::angleBetweenContainers;
    if (rotation > 180)
    {
        rotation = -1 * (360 - rotation);
    }
    debugV("motion going to container starting rotation, scaledRpm: %f rotated: %f", scaledRpm, rotation);

    setSpeedControl(true);
    stepper.setRPM(scaledRpm);
    stepper.rotate(rotation);

    debugV("motion going to container finished");
    currentContainer = index;
}