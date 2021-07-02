
#include <utility>

#include "Arduino.h"
#include "freertos/task.h"

#include "config_constants.h"
#include "motion.h"
#include "debug.h"
#include "pins.h"
#include "math_util.h"
#include "free_rtos_util.h"

void Motion::setup() {
  debugV("motion setup setting up stepper pins");
  SPI.begin();

  digitalWrite(pins::stepper::enable, HIGH);
  digitalWrite(pins::stepper::direction, LOW);
  digitalWrite(pins::stepper::step, LOW);
  digitalWrite(pins::stepper::cs, HIGH);

  setupDriver();
  stepper->connectToPins(pins::stepper::step, pins::stepper::direction);
  digitalWrite(pins::stepper::enable, LOW);
  stepper->setStepsPerRevolution(config::stepper::microsteps * config::stepper::stepsPerRotation);

  debugV("motion setup setting up homing");
  setupHoming();

  delay(1000);

  xTaskCreate(toFreeRtosTask(Motion, checkStepperFaultTask),
              "Stepper Motor Fault Check",
              config::blinkTask::stackSize,
              this,
              config::blinkTask::priority,
              nullptr
  );

  debugV("motion setup completed");
}
void Motion::setupDriver() const {
  driver->begin();
  driver->push();
  driver->toff(3);
  driver->tbl(1);
  driver->hysteresis_start(4);
  driver->hysteresis_end(-2);
  driver->rms_current(1200); // mA
  driver->microsteps(config::stepper::microsteps);
  driver->diag0_stall(1);
  driver->THIGH(0);
  driver->semin(5);
  driver->sfilt(true); // Improves SG readout.
  driver->semax(2);
  driver->sedn(0b01);
  driver->rms_current(800);
  driver->sgt(-10);
}

void Motion::setSpeedControl(bool enabled) {
  if (enabled) {
    stepper->setAccelerationInStepsPerSecondPerSecond(rAccelTosAccel(config::stepper::acceleration));
    stepper->setDecelerationInStepsPerSecondPerSecond(rAccelTosAccel(config::stepper::deceleration));
  } else {
    //This library doesn't support disabling acceleration/deceleration, so move them to near instant
    stepper->setAccelerationInStepsPerSecondPerSecond(
        rAccelTosAccel(config::stepper::acceleration * 1.3F));
    stepper->setDecelerationInStepsPerSecondPerSecond(
        rAccelTosAccel(config::stepper::deceleration * 4.0F));
  }

  debugV("motion speed control changed %d", enabled);
}

void Motion::goToContainerAt(const int index) {
  debugV("motion going to container, index %d", index);

  debugV("motion going to container starting homing");

  goToHome(false);

  debugV("motion going to container homed");
  float scaledRpm = mapFloat(static_cast<float>(index), 0, config::physical::containerCount - 1,
                             config::motion::rpmContainerTravelMin,
                             config::motion::rpmContainerTravelMax);
  float rotation = static_cast<float>(index) * config::motion::angleBetweenContainers;
  if (rotation > 180) {
    rotation = -1 * (360 - rotation);
  }
  debugV(
      "motion going to container starting rotation, scaledRpm: %f rotated: %f",
      rpmToSps(scaledRpm), degToRev(rotation));

  setSpeedControl(true);
  //TODO: Replace with scaled RPM
  stepper->setSpeedInStepsPerSecond(rpmToSps(scaledRpm));
  stepper->moveRelativeInRevolutions(degToRev(rotation));

  debugV("motion going to container finished");
  currentContainer = index;
}
[[noreturn]] void Motion::checkStepperFaultTask() {
  debugV("stepper fault task starting");
  for (;;) {
    delay(3000);
  }
}