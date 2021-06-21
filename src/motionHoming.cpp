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

TaskHandle_t Motion::homingSensorTaskHandle = nullptr;

void Motion::homingSensorTaskWrapper(void *_this)
{
    static_cast<Motion *>(_this)->homingSensorTask();
}

void Motion::setupHoming()
{
    pinMode(pins::sensors::homingHallEffect, INPUT);
    attachInterrupt(digitalPinToInterrupt(pins::sensors::homingHallEffect), Motion::homingSensorIsr, CHANGE);

    xTaskCreate(
        Motion::homingSensorTaskWrapper, // Function that should be called
        "UpdateHallSensorStatus",        // Name of the task (for debugging)
        4024,                            // Stack size (bytes)
        this,                            // Parameter to pass
        1,                               // Task priority
        &Motion::homingSensorTaskHandle  // Task handle
    );
}

void Motion::homingSensorTask()
{
    debugV("homing sensor task started");
    for (;;)
    {
        debugV("homing sensor suspending");
        //Waits for task notification from ISR
        if (!ulTaskNotifyTake(pdTRUE, pdMS_TO_TICKS(300000)))
        {
            debugV("homing sensor task timed out");
        }
        debugV("homing sensor task resumed by isr");

        bool isTriggered = isHomeSensorTriggered();

        debugV("homing sensor task found sensor state: %d", isTriggered);

        if (onHomeStatusChanged)
        {
            onHomeStatusChanged(isTriggered);
            debugV("homing sensor task ran onHomeStatusChanged");
        }

        digitalWrite(pins::onboardLed, isTriggered);
    }
}

bool Motion::isHomeSensorTriggered()
{
    return !digitalRead(pins::sensors::homingHallEffect);
}

void IRAM_ATTR Motion::homingSensorIsr()
{
    if (homingSensorTaskHandle == nullptr)
    {
        return;
    }
    BaseType_t higherPriorityTaskWoke = pdFALSE;
    vTaskNotifyGiveFromISR(homingSensorTaskHandle, &higherPriorityTaskWoke);
    if (higherPriorityTaskWoke != pdFALSE)
    {
        portYIELD_FROM_ISR();
    }
}

void Motion::internalHoming(float rpm, bool reverse)
{
    debugV("internal homing starting, rpm:%f, reverse:%d", rpm, reverse);
    bool foundHome = false;
    onHomeStatusChanged = [this, &foundHome](bool isHome) mutable
    {
        if (isHome)
        {
            foundHome = true;
            stepper.startBrake();
            onHomeStatusChanged = nullptr;
            debugV("internal homing found home");
        }
    };

    stepper.setRPM(rpm);

    unsigned long homingStart = millis();
    float fullRotation = reverse ? -360 : 360;
    while (!foundHome && millis() - homingStart <= config::motion::maxHomingDurationMs)
    {
        debugV("internal homing starting rotations");
        stepper.rotate(fullRotation);
    }
}

void Motion::goToHome(bool forceHoming)
{
    debugV("motion homing started, forceHoming: %d", forceHoming);

    currentContainer = 0;
    if (!forceHoming)
    {

        if (isHomeSensorTriggered())
        {
            debugV("motion homing early exit, was already on home position");
            return;
        }
        stepper.setRPM(config::motion::rpmHomingTravel);
        stepper.rotate(-(currentContainer * config::motion::angleBetweenContainers));
        if (isHomeSensorTriggered())
        {
            debugV("motion homing early exit, home found after reverse rotation");
            return;
        }
        debugV("motion homing continuing instead of early exit");
    }

    debugV("motion homing starting travel");
    internalHoming(config::motion::rpmHomingTravel, false);

    debugV("motion homing travel completed, delaying for stop");
    delay(config::motion::coastingDurationMs); //Allow motor to coast to a stop, avoid the yeet.

    setSpeedControl(false);

    debugV("motion homing starting correction");
    internalHoming(config::motion::rpmHomingCorrection, true);

    setSpeedControl(true);
    debugV("motion homing completed");
}
