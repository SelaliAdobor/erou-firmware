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
#include <api.h>
#include "esp_sntp.h"
#include "commands.h"

TMC2130Stepper driver(pins::stepper::cs);

ESP_FlexyStepper stepper;

Motion motion = Motion(&driver, &stepper); // NOLINT(cppcoreguidelines-slicing)
AsyncWebServer server(80);
ContainerManager containerManager = ContainerManager(&storedSettings);
Api api = Api(&containerManager);

void setupDebugCommands();

#include <TMC2130_bitfields.h>
#include <requestUtil.h>
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
  containerManager.setup();

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
  DebugCommands::setup(&motion);
}

void setupWebServer() {
  server.begin();
  server.on("/reset", HTTP_GET, [](AsyncWebServerRequest *request) {
    storedSettings.reset();
    auto* response = cJSON_CreateObject();
    cJSON_AddBoolToObject(response, "clearedSettings", 1);
    replyWithJson(request, 200, response);
  });
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    auto* response = cJSON_CreateObject();
    cJSON_AddBoolToObject(response, "helloWorld", 1);
    replyWithJson(request, 200, response);
  });
  api.setup(server);
}