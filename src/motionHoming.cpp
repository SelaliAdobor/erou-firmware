#include <Arduino.h>
#include "DRV8825.h"
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <limits>
#include <functional>
#include "pins.h"
#include "config.h"
#include "motion.h"

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
        1000,                            // Stack size (bytes)
        this,                            // Parameter to pass
        1,                               // Task priority
        &Motion::homingSensorTaskHandle  // Task handle
    );
}

void Motion::homingSensorTask()
{
    for (;;)
    {
        //Waits for task notification from ISR
        ulTaskNotifyTake(pdTRUE, pdMS_TO_TICKS(300000));

        bool isTriggered = isHomeSensorTriggered();
        if (onHomeStatusChanged)
        {
            onHomeStatusChanged(isTriggered);
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

void home(float rpm, bool reverse)
{
    bool foundHome = false;
    onHomeStatusChanged = [this, &foundHome](bool isHome) mutable
    {
        if (isHome)
        {
            foundHome = true;
            stepper.startBrake();
            onHomeStatusChanged = nullptr;
        }
    };

    stepper.setRPM(rpm);

    unsigned long homingStart = millis();
    float fullRotation = reverse ? -360 : 360 while (!foundHome && millis() - homingStart <= config::motion::maxHomingDurationMs)
    {
        stepper.rotate(fullRotation);
    }
}

void Motion::goToHome(bool forceHoming)
{

    home(config::motion::rpmHomingTravel, false)

    delay(config::motion::coastingDurationMs); //Allow motor to coast to a stop, avoid the yeet.

    setSpeedControl(false);

    home(config::motion::rpmHomingCorrection, true)

    setSpeedControl(true);
}
