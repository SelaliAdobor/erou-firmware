#include <Arduino.h>
#include "DRV8825.h"
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <limits>
#include <functional>
#include "pins.h"
#include "config.h"
#include "motion.h"
#include "ota.h"
#include "ota_credentials.h"
#include "debug.h"
#include "debug.h"
#include "soc/soc.h"
#include "soc/soc.h"
#include "esp_wifi.h"
#include "esp32-hal.h"
DRV8825 driver(config::stepper::stepsPerRotation,
               pins::stepper::direction,
               pins::stepper::step,
               pins::stepper::enable,
               pins::stepper::m0,
               pins::stepper::m1,
               pins::stepper::m2);

Ota *ota;
Motion motion = Motion(driver);

void runningBlinkTask(void *unused)
{
  debugD("blink task starting");
  for (;;)
  {
    digitalWrite(pins::onboardLed, HIGH);
    delay(config::blinkTask::blinkInterval);
    digitalWrite(pins::onboardLed, LOW);
    delay(config::blinkTask::blinkInterval);
  }
}

void secondaryLoopTask(void *unused)
{
  for (;;)
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
}

void setupWifi()
{

  Serial.begin(config::serial::serialBaud);
  WiFi.mode(WIFI_STA);

  //Helps manage wifi tx power
  esp_wifi_set_max_tx_power(8);
  WiFi.begin(ssid, password);

  while (WiFi.waitForConnectResult() != WL_CONNECTED)
  {
    debugD("setting up wifi");
    delay(5000);
    ESP.restart();
  }
}

void setup()
{
  //In some cases code can cause the entire board to restart,
  //This delay allows a window for new programming
  delay(config::startupDelay);

  setupWifi();

  debugSetup();

  debugD("setting up pins");
  pins::setup();

  debugD("setting up motions");
  motion.setup();

  if (config::blinkTask::enabled)
  {
    debugD("setting up blink");
    xTaskCreate(
        runningBlinkTask,             // Function that should be called
        "Blink While Running",        // Name of the task (for debugging)
        config::blinkTask::stackSize, // Stack size (bytes)
        nullptr,                      // Parameter to pass
        config::blinkTask::priority,  // Task priority
        nullptr                       // Task handle
    );
  }

  debugD("starting secondary loop task");
  TaskHandle_t loopHandle;
  //Allows ArduinoOTA to use system loop
  xTaskCreateUniversal(
      secondaryLoopTask,           // Function that should be called
      "Loop Task",                 // Name of the task (for debugging)
      config::loopTask::stackSize, // Stack size (bytes)
      nullptr,                     // Parameter to pass
      config::loopTask::priority,  // Task priority
      &loopHandle,                 // Task handle
      config::loopTask::core);

  debugD("setting up ota");
  ota = new Ota(loopHandle);
  ota->setup();

  debugI("setup complete");
}

void loop()
{
  ota->loop();
  debugLoop();
  delay(50);
}
