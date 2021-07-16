#pragma once

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include <ESP_FlexyStepper.h>
#include <functional>
#include <TMCStepper.h>
#include <memory>
#include <utility>
#include "config_constants.h"
//Convert acceleration in revolutions to steps
inline float rAccelTosAccel(float rAccel) {
  return (rAccel * config::stepper::stepsPerRotation * config::stepper::microsteps) / 60.0F;
}
//Convert angle in degrees to revolutions
inline float degToRev(float degrees) { return degrees / 360.0F; }
//Convert speed in rpm to steps per second
inline float rpmToSps(float rpm) {
  return (rpm * config::stepper::stepsPerRotation * config::stepper::microsteps) / 60.0F;
}

class Motion {
 private:
  int currentContainer = 0;

  TMC2130Stepper *driver;
  ESP_FlexyStepper *stepper;

  std::function<void(bool)> onHomeStatusChanged = nullptr;

  void setSpeedControl(bool enabled);
  void setupHoming();
  void internalHoming(float rpm, bool reverse);


  static bool isHomeSensorTriggered();
  static void homingSensorIsr();

  static TaskHandle_t homingSensorTaskHandle;
  [[noreturn]] void homingSensorTask();

  [[noreturn]] inline void checkStepperFaultTask();
 public:
  Motion(TMC2130Stepper *d, ESP_FlexyStepper *s) :
      driver(d), stepper(s) {}

  void setup();
  void goToHome(bool forceHoming);
  void goToContainerAt(int index);
  void setupDriver() const;
};
