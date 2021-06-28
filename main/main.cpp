#include <functional>
#include <limits>
#include <sstream>
#include <new>

#include "Arduino.h"
#include "freertos/task.h"
#include "freertos/FreeRTOS.h"
#include "ESPAsyncWebServer.h"
#include "DRV8825.h"
#include "config_constants.h"
#include "debug.h"
#include "esp32-hal.h"
#include "esp_wifi.h"
#include "motion.h"
#include "ota.h"
#include "ota_credentials.h"
#include "pins.h"

DRV8825 driver(config::stepper::stepsPerRotation, pins::stepper::direction,
               pins::stepper::step, pins::stepper::enable, pins::stepper::m0,
               pins::stepper::m1, pins::stepper::m2);

Ota *ota;
Motion motion = Motion(static_cast<BasicStepperDriver>(driver)); // NOLINT(cppcoreguidelines-slicing)
AsyncWebServer server(80);
void setupDebugCommands();

[[noreturn]] void runningBlinkTask(void *) {
  debugV("blink task starting");
  for (;;) {
    // digitalWrite(pins::onboardLed, HIGH);
    delay(config::blinkTask::blinkInterval);
    //  digitalWrite(pins::onboardLed, LOW);
    delay(config::blinkTask::blinkInterval);
  }
}

[[noreturn]] void checkStepperFaultTask(void *) {
  debugV("stepper fault task starting");
  for (;;) {
    delay(config::blinkTask::blinkInterval);
    if (digitalRead(pins::stepper::fault) == LOW) {
      debugE("stepper motor drive fault detected!");
    }
  }
}

[[noreturn]]  void secondaryLoopTask(void *) {
  for (;;) {
    delay(500);
  }
}

void setupWifi() {
  Serial.begin(config::serial::baud);
  WiFiClass::mode(WIFI_STA);

  // Helps manage wifi tx power
  esp_wifi_set_max_tx_power(8);
  WiFi.begin(ssid, password);

  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    debugV("setting up wifi");
    delay(5000);
    ESP.restart();
  }
}

extern "C" {
void app_main(void) {
  initArduino();
  setup();
}
}

void setup() {
  setupWifi();
  // In some cases code can cause the entire board to restart,
  // This delay allows a window for new programming
  delay(config::startupDelay);

  debugInstance.setup(server);
  setupDebugCommands();
  server.begin();
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/plain", "Hello, world");
  });
  if (config::blinkTask::enabled) {
    debugI("setting up blink");
    xTaskCreate(runningBlinkTask,
                "Blink While Running",
                config::blinkTask::stackSize,
                nullptr,
                config::blinkTask::priority,
                nullptr
    );
  }

  delay(config::startupDelay);
  debugI("setting up pins");
  pins::setup();

  debugI("setting up motion");
  motion.setup();
  xTaskCreate(checkStepperFaultTask,
              "Stepper Motor Fault Check",
              config::blinkTask::stackSize,
              nullptr,
              config::blinkTask::priority,
              nullptr
  );

  debugI("starting secondary loop task");
  TaskHandle_t loopHandle;
  xTaskCreateUniversal(secondaryLoopTask,
                       "Loop Task",
                       config::loopTask::stackSize,
                       nullptr,
                       config::loopTask::priority,
                       &loopHandle,
                       config::loopTask::core);

  debugI("setting up ota");
  Ota::setup();

  debugI("setup complete");
}

void setupDebugCommands() {
  debugInstance.registerCommand("en ", [](const std::string *args) {
    uint8_t enabled = 0;
    if (sscanf(args->c_str(), "%" SCNu8, &enabled) != 1) {
      debugI("Failed to parse command.");
      return;
    }

    digitalWrite(pins::stepper::enable, enabled);
    debugI("Updated stepper motor enabled state %d", enabled);
  });

  debugInstance.registerCommand("gtc ", [](const std::string *args) {
    int containerIndex = 0;

    if (sscanf(args->c_str(), "%d", &containerIndex) != 1) {
      debugI("Failed to parse command.");
      return;
    }

    debugI("Going to container %d for debug command", containerIndex);
    if (containerIndex >= config::physical::containerCount) {
      debugE(
          "Container index exceeded for goToContainer: %d out of %d possible",
          containerIndex, config::physical::containerCount);
      return;
    }
    motion.goToContainerAt(containerIndex);
    debugI("Went to container %d after debug command", containerIndex);
  });

}