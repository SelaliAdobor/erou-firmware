#pragma once

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include <functional>

#include "BasicStepperDriver.h"

class Motion {
 private:
  BasicStepperDriver stepper;
  std::function<void(bool)> onHomeStatusChanged = nullptr;
  int currentContainer = 0;

  void setSpeedControl(bool enabled);

  void setupHoming();
  bool isHomeSensorTriggered();
  void homingSensorTask();
  void internalHoming(float rpm, bool reverse);

  static TaskHandle_t homingSensorTaskHandle;
  inline static void homingSensorTaskWrapper(void *);

  static void homingSensorIsr();

 public:
  Motion(BasicStepperDriver stepper);

  void setup();
  void goToHome(bool forceHoming);
  void goToContainerAt(int index);
};
