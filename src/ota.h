#pragma once

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

class Ota
{
private:
    TaskHandle_t secondaryLoopHandle;

public:
    Ota(TaskHandle_t handle);

    void setup();
    void loop();
};