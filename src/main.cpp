#include <Arduino.h>
#include "DRV8825.h"
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <limits>
#include <functional>
#include "pins.h"
#include "config.h"
#include "motion.h"

DRV8825 driver(config::stepper::stepsPerRotation,
               pins::stepper::direction,
               pins::stepper::step,
               pins::stepper::enable,
               pins::stepper::m0,
               pins::stepper::m1,
               pins::stepper::m2);

Motion motion = Motion(driver);

void runningBlinkTask(void* unused)
{
  for (;;)
  {
    digitalWrite(pins::onboardLed, HIGH);
    delay(config::runningBlinkInterval);
    digitalWrite(pins::onboardLed, LOW);
    delay(config::runningBlinkInterval);
  }
}

void setup()
{
  Serial.begin(115200);

  pins::setup();

  if (config::runningBlink)
  {
    xTaskCreate(
        runningBlinkTask,      // Function that should be called
        "Blink While Running", // Name of the task (for debugging)
        1000,                  // Stack size (bytes)
        nullptr,               // Parameter to pass
        1,                     // Task priority
        nullptr                // Task handle
    );
  }
  motion.setup();
}

void loop()
{
  motion.goToHome(true);
  delay(1000);
  for (int i = 0; i < 8; i++)
  {
    motion.goToContainerAt(i);
    delay(1000);
  }
  driver.disable();
  delay(6000);
  driver.enable();
}
