#pragma once

#include <ArduinoOTA.h>
#include <ESPmDNS.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

class Ota {
 private:
  [[noreturn]]  static void otaTask(void *);

 public:
  static void setup();
};