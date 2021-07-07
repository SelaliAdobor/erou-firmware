#pragma once

namespace pins {
const int onboardLed = 2;

namespace stepper {
//CSN/CS=>2
//SCK =>18*
//SDI/MOSI => 23*
//SDO/MISO => 19
//EN=>27
const int enable = 27;

const int cs = 5;//SDI/MOSI
const int sck = 18;//SCK
const int mosi = 23;//SCK
const int miso = 19;//SCK


const int direction = 33;
const int step = 32;

}  // namespace stepper

namespace sensors {

const int homingHallEffect = 35;
}

inline void setup() {
  pinMode(pins::onboardLed, OUTPUT);

  pinMode(pins::stepper::enable, OUTPUT);

  pinMode(pins::stepper::cs, OUTPUT);
  pinMode(pins::stepper::sck, OUTPUT);

  pinMode(pins::stepper::mosi, INPUT);
  pinMode(pins::stepper::miso, INPUT_PULLUP);


  pinMode(pins::stepper::direction, OUTPUT);
  pinMode(pins::stepper::step, OUTPUT);
}
}  // namespace pins
