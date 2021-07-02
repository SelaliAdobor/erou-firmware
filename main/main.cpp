#include <functional>

#include "Arduino.h"
#include "freertos/task.h"
#include "ESPAsyncWebServer.h"
#include "config_constants.h"
#include "debug.h"
#include "esp32-hal.h"
#include "esp_wifi.h"
#include "motion.h"
#include "ota.h"
#include "ota_credentials.h"
#include "pins.h"
#include <bitset>

TMC2130Stepper driver(pins::stepper::cs);

ESP_FlexyStepper stepper;
Ota *ota;

Motion motion = Motion(&driver, &stepper); // NOLINT(cppcoreguidelines-slicing)
AsyncWebServer server(80);
void setupDebugCommands();

#include <TMC2130_bitfields.h>
void setupWebServer();
void setupDebug();
[[noreturn]] void runningBlinkTask(void *) {
  debugV("blink task starting");
  for (;;) {
    // digitalWrite(pins::onboardLed, HIGH);
    delay(config::blinkTask::blinkInterval);
    //  digitalWrite(pins::onboardLed, LOW);
    delay(config::blinkTask::blinkInterval);
  }
}

[[noreturn]]  void secondaryLoopTask(void *) {
  for (;;) {
    delay(5000);
    debugI( "RAM left %d", esp_get_free_heap_size());
    debugI( "Stack used %d", uxTaskGetStackHighWaterMark(NULL));
    debugI("Stall detected? %d %s", int(driver.stallguard()), std::bitset<32>(driver.DRV_STATUS()).to_string());
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
  pins::setup();
  setupWifi();
  Ota::setup();
  setupDebug();
  setupWebServer();
  debugI("Setup waiting for debug connections");
  delay(10000);

  debugI("setting up motion");
  motion.setup();

  debugI("starting secondary loop task");
  TaskHandle_t loopHandle;
  xTaskCreateUniversal(secondaryLoopTask,
                       "Loop Task",
                       config::loopTask::stackSize,
                       nullptr,
                       config::loopTask::priority,
                       &loopHandle,
                       config::loopTask::core);

  debugI("setup complete");
}
void setupDebug() {
  debugInstance.setup(server);
  debugInstance.registerCommand("en ", [](const std::string *args) {
    uint8_t enabled = 0;
    if (sscanf(args->c_str(), "%" SCNu8, &enabled) != 1) {
      debugI("Failed to parse command.");
      return;
    }

    digitalWrite(pins::stepper::enable, !enabled);
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
  debugInstance.registerCommand("ut", [](const std::string *) {
    debugI("%d seconds since last reboot", int(esp_timer_get_time() / 1000));
  });
}
void setupWebServer() {
  server.begin();
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/plain", "Hello, world");
  });
}