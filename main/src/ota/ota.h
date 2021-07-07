#pragma once

#include <ArduinoOTA.h>
#include <ESPmDNS.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "ota_credentials.h"
/*
Expects a file named "ota_credentials.h" to be created in the same directory as this header.
Content should match this format:
-----------------------------------------------------------
#pragma once

const char *const ssid = "<your wifi network>";
const char *const password = "<your wifi password>";
-----------------------------------------------------------
 */
class Ota {
 private:
  [[noreturn]]  static void otaTask(void *);
  static TaskHandle_t otaTaskHandle;
 public:
  static void setup();
};