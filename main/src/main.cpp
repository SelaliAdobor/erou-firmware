#include <functional>
#include "SPIFFS.h"
#include "Arduino.h"
#include "freertos/task.h"
#include "ESPAsyncWebServer.h"
#include "config_constants.h"
#include "wsdebug.h"
#include "esp32-hal.h"
#include "esp_wifi.h"
#include "motion.h"
#include "ota/ota.h"
#include "ota/ota_credentials.h"
#include "pins.h"
#include <bitset>
#include <containerManager.h>
#include <cinttypes>
#include <chrono>
#include <croncpp.h>
#include "esp_sntp.h"

TMC2130Stepper driver(pins::stepper::cs);

ESP_FlexyStepper stepper;
Ota *ota;

Motion motion = Motion(&driver, &stepper); // NOLINT(cppcoreguidelines-slicing)
AsyncWebServer server(80);
ContainerManager contentManager = ContainerManager(&storedSettings);
void setupDebugCommands();

#include <TMC2130_bitfields.h>
void setupWebServer();
void setupDebug();

[[noreturn]]  void secondaryLoopTask(void *) {
  for (;;) {
    delay(1000);


    //  debugI("Now %d %s Next %s Remaining %2ld hours %2ld minutes %2ld seconds",touchRead(4), LocalTimezone.dateTime(now, "d m Y H:M").c_str(), LocalTimezone.dateTime(next, "d m Y H:M").c_str(), hours, minutes, seconds );
  }
}

void setupWifi() {
  Serial.begin(config::serial::baud);
  WiFiClass::mode(WIFI_STA);
  // Helps manage wifi tx power
  esp_wifi_set_max_tx_power(34);
  esp_wifi_set_ps(WIFI_PS_NONE);
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
  SPIFFS.begin(true);
  storedSettings.setup();

  const char *timezoneSettingsKey = "config/timezone";
  if (auto storedTimezone = storedSettings.getString(timezoneSettingsKey)) {
    configTzTime(storedTimezone.value(), config::network::ntpServer);
  }

  Ota::setup();
  setupDebug();
  setupWebServer();

  debugI("setting up container manager");
  contentManager.setup();

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
    debugI("%d seconds since last reboot", int (esp_timer_get_time() / 1000000));
  });
}
void setupWebServer() {
  server.begin();
  server.on("/reset", HTTP_GET, [](AsyncWebServerRequest *request) {
    storedSettings.reset();
    request->send(200, "text/plain", "Stored Settings Cleared.");
  });
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/plain", "Hello, world");
  });
}