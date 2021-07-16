#include "ota.h"

#include "wsdebug.h"

TaskHandle_t Ota::otaTaskHandle = nullptr;

void Ota::setup() {

  ArduinoOTA.setHostname("erou");

  ArduinoOTA
      .onStart([]() {
        vTaskPrioritySet(otaTaskHandle, 18);
        if (ArduinoOTA.getCommand() == U_FLASH) {
          debugI("Start updating via flash");
        } else {
          debugI("Start updating via spiffs");
        }
      })
      .onEnd([]() {
        debugI("\n\nEnd");
      })
      .onProgress([](unsigned int progress, unsigned int total) {
        // I don't like how it formats otherwise
        debugI("Progress: %u%%\r\033[1A'", (progress / (total / 100)));
      })
      .onError([](ota_error_t error) {
        debugI("\n\nError[%u]: %s ", error, Update.errorString());
        switch (error) {
          case OTA_AUTH_ERROR: debugI("Auth Failed");
            break;
          case OTA_BEGIN_ERROR: debugI("Begin Failed");
            break;
          case OTA_CONNECT_ERROR: debugI("Connect Failed");
            break;
          case OTA_RECEIVE_ERROR: debugI("Receive Failed");
            break;
          case OTA_END_ERROR: debugI("End Failed");
            break;
          default: debugI("Unknown Failture");
            break;
        }
      });

  ArduinoOTA.begin();

  debugI("Ready");
  debugI("IP address: %s", WiFi.localIP().toString().c_str());

  xTaskCreate(
      otaTask,          /* Task function. */
      "OTA Task",        /* String with name of task. */
      10000,            /* Stack size in bytes. */
      nullptr,             /* Parameter passed as input of the task */
      1,                /* Priority of the task. */
      &otaTaskHandle);            /* Task handle. */
}

[[noreturn]] void Ota::otaTask(void *) {
  for (;;) {
    ArduinoOTA.handle();
    delay(3500);
  }
}