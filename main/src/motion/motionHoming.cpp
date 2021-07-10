#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include <functional>
#include <limits>

#include "config_constants.h"
#include "wsdebug.h"
#include "motion.h"
#include "free_rtos_util.h"
#include "pins.h"

TaskHandle_t Motion::homingSensorTaskHandle = nullptr;

void Motion::setupHoming() {
  pinMode(pins::sensors::homingHallEffect, INPUT);
  attachInterrupt(digitalPinToInterrupt(pins::sensors::homingHallEffect),
                  Motion::homingSensorIsr, CHANGE);

  xTaskCreate(
      toFreeRtosTask(Motion, homingSensorTask),  // Function that should be called
      "UpdateHallSensorStatus",         // Name of the task (for debugging)
      4024,                             // Stack size (bytes)
      this,                             // Parameter to pass
      1,                                // Task priority
      &Motion::homingSensorTaskHandle   // Task handle
  );
}

[[noreturn]] void Motion::homingSensorTask() {
  debugV("homing sensor task started");
  for (;;) {
    // Waits for task notification from ISR
    if (!ulTaskNotifyTake(pdTRUE, pdMS_TO_TICKS(300000))) {
      debugV("homing sensor task timed out");
    }

    bool isTriggered = isHomeSensorTriggered();

    if (onHomeStatusChanged) {
      debugV("homing sensor task found sensor state: %d", isTriggered);
      onHomeStatusChanged(isTriggered);
    }

    digitalWrite(pins::onboardLed, isTriggered);
  }
}

bool Motion::isHomeSensorTriggered() {
  return digitalRead(pins::sensors::homingHallEffect) == 0;
}

void IRAM_ATTR Motion::homingSensorIsr() {
  if (homingSensorTaskHandle == nullptr) {
    return;
  }
  BaseType_t higherPriorityTaskWoke = pdFALSE;
  vTaskNotifyGiveFromISR(homingSensorTaskHandle, &higherPriorityTaskWoke);
  if (higherPriorityTaskWoke != pdFALSE) {
    portYIELD_FROM_ISR();
  }
}

void Motion::internalHoming(float rpm, bool reverse) {
  debugV("internal homing starting, rpm:%f, reverse:%d", rpm, reverse);
  bool foundHome = false;
  onHomeStatusChanged = [this, &foundHome](bool isHome) mutable {
    if (isHome) {
      foundHome = true;

      setSpeedControl(false);
      stepper->setTargetPositionToStop();
      while (!stepper->processMovement()) { vTaskDelay(1); }
      onHomeStatusChanged = nullptr;
      debugV("internal homing found home");
    }
  };

  stepper->setSpeedInStepsPerSecond(rpmToSps(rpm));

  uint32_t homingStart = millis();
  float fullRotation = reverse ? -360 : 360;
  setSpeedControl(true);
  while (!foundHome &&
      millis() - homingStart <= config::motion::maxHomingDurationMs) {

    debugV("internal homing starting rotations");
    stepper->moveRelativeInRevolutions(degToRev(fullRotation));
  }

  if (!foundHome) {
    debugE("failed to find homing!");
  }
}

void Motion::goToHome(bool forceHoming) {
  debugV("motion homing started, forceHoming: %d", forceHoming);
  setSpeedControl(true);
  if (!forceHoming) {
    if (isHomeSensorTriggered()) {
      debugI("motion homing early exit, was already on home position");
      return;
    }
    stepper->setSpeedInStepsPerSecond(rpmToSps(config::motion::rpmHomingCorrection));
    float reversingAngle =
        -(static_cast<float>(currentContainer) * config::motion::angleBetweenContainers);
    float reversingAngleAbsolute = abs(reversingAngle);
    if (reversingAngleAbsolute > 180) {
      reversingAngle = 360 - reversingAngleAbsolute;
    }
    stepper->moveRelativeInRevolutions(reversingAngle / 360.0F);
    if (isHomeSensorTriggered()) {
      debugI("motion homing early exit, home found after reverse rotation");
      return;
    }
    debugV("motion homing continuing instead of early exit");
  }

  debugV("motion homing starting travel");
  internalHoming(config::motion::rpmHomingTravel, false);

  debugV("motion homing travel completed, delaying for stop");
  delay(config::motion::coastingDurationMs);  // Allow motor to coast to a stop,
  // avoid the yeet.
  stepper->setSpeedInStepsPerSecond(rpmToSps(config::motion::rpmHomingCorrection));
  stepper->moveRelativeInRevolutions(degToRev(config::motion::homingCorrectionOvershoot));

  debugV("motion homing starting correction");
  internalHoming(config::motion::rpmHomingCorrection, true);

  debugI("motion homing completed");
}
