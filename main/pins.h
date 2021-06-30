#pragma once

namespace pins {
const int onboardLed = 2;

namespace stepper {
const int enable = 27;

const int ms1 = 23;
const int ms2 = 18;

const int pdnTx = 5;
const int pdnRx = 26;

const int standby = 21;
const int clock = 25;

const int direction = 33;
const int step = 32;

}  // namespace stepper

namespace sensors {

const int homingHallEffect = 35;
}

inline void setup() {
  pinMode(pins::onboardLed, OUTPUT);

  pinMode(pins::stepper::ms1, OUTPUT);
  pinMode(pins::stepper::ms2, OUTPUT);

  pinMode(pins::stepper::pdnTx, INPUT);
  pinMode(pins::stepper::pdnRx, OUTPUT);

  pinMode(pins::stepper::standby, OUTPUT);
  pinMode(pins::stepper::clock, OUTPUT);

  pinMode(pins::stepper::direction, OUTPUT);
  pinMode(pins::stepper::step, OUTPUT);
}
}  // namespace pins
