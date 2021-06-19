#include <Arduino.h>
#include "DRV8825.h"
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <limits>
#include <functional>

constexpr int stepsPerRotation = 200;
constexpr int revolutionsPerMinute = 35;
constexpr int microsteps = 32;

#define PIN_ONBOARD_LED 2

#define PIN_ENABLE 27 // Enable
#define PIN_RESET 21  // Enable
#define PIN_SLEEP 25  // Enable

#define PIN_DIRECTION 33 // Direction
#define PIN_STEP 32      // Step // Chip select

#define PIN_M0 23
#define PIN_M1 18
#define PIN_M2 5

#define PIN_FAULT 5

#define PIN_HOMING_HALL 35

#define MOTOR_ACCEL 1000
#define MOTOR_DECEL 1000

TaskHandle_t homingSensorTask = nullptr;

boolean isCurrentPositionHome = false;

DRV8825 driver(stepsPerRotation, PIN_DIRECTION, PIN_STEP, PIN_ENABLE, PIN_M0, PIN_M1, PIN_M2);

void IRAM_ATTR homePositionIsr()
{
  if (homingSensorTask == nullptr)
  {
    return;
  }
  BaseType_t higherPriorityTaskWoke = pdFALSE;
  vTaskNotifyGiveFromISR(homingSensorTask, &higherPriorityTaskWoke);
  if (higherPriorityTaskWoke != pdFALSE)
  {
    portYIELD_FROM_ISR();
  }
}

std::function<void(bool)> onHomeStatusChanged = nullptr;

bool checkIfHome()
{
  return !digitalRead(PIN_HOMING_HALL);
}

void updateHallSensorStatus(void *unused)
{
  for (;;)
  {
    ulTaskNotifyTake(pdTRUE, pdMS_TO_TICKS(300000));
    isCurrentPositionHome = checkIfHome();
    if (onHomeStatusChanged != nullptr)
    {
      onHomeStatusChanged(isCurrentPositionHome);
    }
    digitalWrite(PIN_ONBOARD_LED, isCurrentPositionHome);
  }
}
void setSpeedControl(bool enabled)
{
  if (enabled)
  {
    driver.setSpeedProfile(driver.LINEAR_SPEED, MOTOR_ACCEL, MOTOR_DECEL);
  }
  else
  {
    driver.setSpeedProfile(driver.CONSTANT_SPEED, MOTOR_ACCEL, MOTOR_DECEL);
  }
}
void setup()
{
  pinMode(PIN_ENABLE, OUTPUT);
  pinMode(PIN_RESET, OUTPUT);
  pinMode(PIN_SLEEP, OUTPUT);

  pinMode(PIN_ONBOARD_LED, OUTPUT);

  pinMode(PIN_HOMING_HALL, INPUT);

  pinMode(PIN_DIRECTION, OUTPUT);
  pinMode(PIN_STEP, OUTPUT);

  pinMode(PIN_M0, OUTPUT);
  pinMode(PIN_M1, OUTPUT);
  pinMode(PIN_M2, OUTPUT);

  pinMode(PIN_FAULT, INPUT_PULLUP);

  digitalWrite(PIN_ENABLE, LOW);
  digitalWrite(PIN_SLEEP, HIGH);
  digitalWrite(PIN_RESET, HIGH);

  delay(1000);

  driver.setEnableActiveState(LOW);
  driver.begin(revolutionsPerMinute, microsteps);
  
  driver.enable();
  attachInterrupt(digitalPinToInterrupt(PIN_HOMING_HALL), homePositionIsr, CHANGE);
  xTaskCreate(
      updateHallSensorStatus,   // Function that should be called
      "UpdateHallSensorStatus", // Name of the task (for debugging)
      1000,                     // Stack size (bytes)
      NULL,                     // Parameter to pass
      1,                        // Task priority
      &homingSensorTask         // Task handle
  );
}

void home()
{
  bool foundHome = false;
  onHomeStatusChanged = [&foundHome](bool isHome) mutable
  {
    if (isHome)
    {
      foundHome = true;
      driver.startBrake();
      onHomeStatusChanged = nullptr;
    }
  };
  //Todo: Add timeout
  while (!foundHome)
  {
    driver.rotate(360);
  }

  delay(200); //Allow motor to coast to a stop, avoid the yeet.

  setSpeedControl(false);
  while (!checkIfHome())
  {
    driver.move(-1);
    delay(10);
  }
  setSpeedControl(true);
}

void loop()
{
  home();
  delay(1000);
  for(int i = 0; i < 8; i++){
    driver.rotate(45);
    delay(1000);
  }
  driver.disable();
  delay(6000);
  driver.enable();
}
