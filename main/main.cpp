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
#include "driver.h"
#include <bitset>

TMC2130Stepper driver(pins::stepper::cs);

ESP_FlexyStepper stepper;
Ota *ota;

Motion motion = Motion(&driver, &stepper); // NOLINT(cppcoreguidelines-slicing)
AsyncWebServer server(80);
void setupDebugCommands();

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
  pins::setup();
  digitalWrite(pins::stepper::enable, LOW);

  setupWifi();
  debugI("setting up ota");
  Ota::setup();
  setupDebug();
  setupWebServer();
  delay(10000);

  delay(100);
for(;;) {
  debugI("got driver version %d, %s", int(driver.version()), std::bitset<32>(driver.IOIN()).to_string());
  delay(1000);
}
Serial2.begin(19200, SERIAL_8N1);


  debugI("setting up pins");

  delay(2000);
  // In some cases code can cause the entire board to restart,
  // This delay allows a window for new programming
  delay(config::startupDelay);
  delay(config::startupDelay);

  delay(config::startupDelay);
  delay(config::startupDelay);
  stepper.connectToPins(pins::stepper::step, pins::stepper::direction);

  delay(5000);

  debugI("setting up motion");
  motion.setup();

  debugI("starting secondary loop task");
  TaskHandle_t loopHandle;
//  xTaskCreateUniversal(secondaryLoopTask,
//                       "Loop Task",
//                       config::loopTask::stackSize,
//                       nullptr,
//                       config::loopTask::priority,
//                       &loopHandle,
//                       config::loopTask::core);

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

}
void setupWebServer() {
  server.begin();
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/plain", "Hello, world");
  });
}