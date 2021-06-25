#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include <functional>
#include <limits>
#include <sstream>

#include "DRV8825.h"
#include "config.h"
#include "debug.h"
#include "esp32-hal.h"
#include "esp_wifi.h"
#include "motion.h"
#include "ota.h"
#include "ota_credentials.h"
#include "pins.h"
#include "soc/soc.h"
DRV8825 driver(config::stepper::stepsPerRotation, pins::stepper::direction,
               pins::stepper::step, pins::stepper::enable, pins::stepper::m0,
               pins::stepper::m1, pins::stepper::m2);

Ota *ota;
Motion motion = Motion(driver);

void runningBlinkTask(void *unused) {
  debugV("blink task starting");
  for (;;) {
    // digitalWrite(pins::onboardLed, HIGH);
    delay(config::blinkTask::blinkInterval);
    //  digitalWrite(pins::onboardLed, LOW);
    delay(config::blinkTask::blinkInterval);
  }
}
void checkStepperFaultTask(void *unused) {
  debugV("stepper fault task starting");
  for (;;) {
    delay(config::blinkTask::blinkInterval);
    if (digitalRead(pins::stepper::fault) == LOW) {
      debugE("stepper motor drive fault detected!");
    }
  }
}

void secondaryLoopTask(void *unused) {
  for (;;) {
    delay(500);
  }
}

void setupWifi() {
  Serial.begin(config::serial::serialBaud);
  WiFi.mode(WIFI_STA);

  // Helps manage wifi tx power
  esp_wifi_set_max_tx_power(8);
  WiFi.begin(ssid, password);

  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    debugV("setting up wifi");
    delay(5000);
    ESP.restart();
  }
}

void setup() {
  setupWifi();
  // In some cases code can cause the entire board to restart,
  // This delay allows a window for new programming
  delay(config::startupDelay);

  DebugInstance.setup();
  DebugInstance.registerCommand("en", [](std::stringstream args) {
    int enabled = 0;
    args >> enabled;

    if (!args.good()) {
      debugI("Failed to parse command.");
      return;
    }
    digitalWrite(pins::stepper::enable, !enabled);
    debugI("Updated stepper motor enabled state %d", enabled);
  });

  DebugInstance.registerCommand("gtc", [](std::stringstream args) {
    int containerIndex = 0;
    args >> containerIndex;

    if (!args.good()) {
      debugI("Failed to parse command.");
      return;
    }
    debugI("Going to container %d for debug command", containerIndex);
    if (containerIndex >= config::containerCount) {
      debugE(
          "Container index exceeded for goToContainer: %d out of %d possible",
          containerIndex, config::containerCount);
      return;
    }
    motion.goToContainerAt(containerIndex);
    debugI("Went to container %d after debug command", containerIndex);
  });

  if (config::blinkTask::enabled) {
    debugI("setting up blink");
    xTaskCreate(runningBlinkTask,       // Function that should be called
                "Blink While Running",  // Name of the task (for debugging)
                config::blinkTask::stackSize,  // Stack size (bytes)
                nullptr,                       // Parameter to pass
                config::blinkTask::priority,   // Task priority
                nullptr                        // Task handle
    );
  }

  delay(config::startupDelay);
  debugI("setting up pins");
  pins::setup();

  debugI("setting up motion");
  motion.setup();
  xTaskCreate(checkStepperFaultTask,         // Function that should be called
              "Stepper Motor Fault Check",   // Name of the task (for debugging)
              config::blinkTask::stackSize,  // Stack size (bytes)
              nullptr,                       // Parameter to pass
              config::blinkTask::priority,   // Task priority
              nullptr                        // Task handle
  );

  debugI("starting secondary loop task");
  TaskHandle_t loopHandle;
  // Allows ArduinoOTA to use system loop
  xTaskCreateUniversal(secondaryLoopTask,  // Function that should be called
                       "Loop Task",        // Name of the task (for debugging)
                       config::loopTask::stackSize,  // Stack size (bytes)
                       nullptr,                      // Parameter to pass
                       config::loopTask::priority,   // Task priority
                       &loopHandle,                  // Task handle
                       config::loopTask::core);

  debugI("setting up ota");
  ota = new Ota(loopHandle);
  ota->setup();

  debugI("setup complete");
}

void loop() {
  ota->loop();
  DebugInstance.loop();
  delay(50);
}
