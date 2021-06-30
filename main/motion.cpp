
#include <utility>

#include "Arduino.h"
#include "freertos/task.h"

#include "config_constants.h"
#include "motion.h"
#include "debug.h"
#include "pins.h"
#include "math_util.h"

void Motion::setup() {
  debugV("motion setup setting up stepper pins");

  stepper->connectToPins(pins::stepper::step, pins::stepper::direction);
  digitalWrite(pins::stepper::enable, LOW);
  digitalWrite(pins::stepper::clock, LOW);
  digitalWrite(pins::stepper::ms1, LOW);
  digitalWrite(pins::stepper::ms2, LOW);
  stepper->setStepsPerRevolution(config::stepper::microsteps * config::stepper::stepsPerRotation);

  driver->begin();                 //  SPI: Init CS pins and possible SW SPI pins
  driver->pdn_disable(true);     // Use PDN/UART pin for communication
  debugI("pdn_disable stepper motor crc status: %d", driver->CRCerror);
  // UART: Init SW UART (if selected) with default 115200 baudrate
  driver->toff(4);                 // Enables driver0b00 in software
  debugI("toff stepper motor crc status: %d", driver->CRCerror);
  driver->blank_time(24);
  debugI("blank_time stepper motor crc status: %d", driver->CRCerror);
  driver->rms_current(config::stepper::current);        // Set motor RMS current
  debugI("rms_current stepper motor crc status: %d", driver->CRCerror);
  driver->microsteps(config::stepper::microsteps);          // Set microsteps to 1/16th
  debugI("microsteps stepper motor crc status: %d", driver->CRCerror);
  driver->en_spreadCycle(false);   // Toggle spreadCycle on TMC2208/2209/2224
  debugI("en_spreadCycle stepper motor crc status: %d", driver->CRCerror);
  driver->pwm_autoscale(true);     // Needed for stealthChop
  debugI("pwm_autoscale motor crc status: %d", driver->CRCerror);

  debugI("stepper motor microsteps status: %d", driver->microsteps());
  debugI("microsteps status: %d", driver->CRCerror);
  debugI("stepper motor version status: %d", driver->version());
  debugI("version status: %d", driver->CRCerror);
  debugV("motion setup setting up homing");
  setupHoming();

  delay(1000);


    xTaskCreate(checkStepperFaultTaskWrapper,
              "Stepper Motor Fault Check",
              config::blinkTask::stackSize,
              this,
              config::blinkTask::priority,
              nullptr
  );

  debugV("motion setup completed");
}

void Motion::setSpeedControl(bool enabled) {
  if (enabled) {
    stepper->setAccelerationInStepsPerSecondPerSecond(rAccelTosAccel(config::stepper::acceleration));
    stepper->setDecelerationInStepsPerSecondPerSecond(rAccelTosAccel(config::stepper::deceleration));
  } else {
    //This library doesn't support disabling acceleration/deceleration, so move them to near instant
    stepper->setAccelerationInStepsPerSecondPerSecond(rAccelTosAccel(config::stepper::acceleration*1.3F));
    stepper->setDecelerationInStepsPerSecondPerSecond(rAccelTosAccel(config::stepper::deceleration*1.3F));
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
      rpmToSps(config::motion::rpmContainerTravelMax), degToRev(rotation));

  setSpeedControl(true);
  //TODO: Replace with scaled RPM
  stepper->setSpeedInStepsPerSecond(rpmToSps(config::motion::rpmContainerTravelMax));
  stepper->moveRelativeInRevolutions(degToRev(rotation));

  debugV("motion going to container finished");
  currentContainer = index;
}
void Motion::checkStepperFaultTaskWrapper(void * _this){
    static_cast<Motion *>(_this)->checkStepperFaultTask();
}
[[noreturn]] void Motion::checkStepperFaultTask() {
  debugV("stepper fault task starting");
  for (;;) {
    delay(3000);
  }
}