#pragma once

#include <ArduinoOTA.h>
#include <ESPmDNS.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

class Ota {
 private:
  TaskHandle_t secondaryLoopHandle;

 public:
  Ota(TaskHandle_t handle);

  void setup();
  void loop();
};