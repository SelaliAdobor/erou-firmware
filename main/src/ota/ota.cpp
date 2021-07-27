#include "ota.h"

#include "wsdebug.h"

TaskHandle_t Ota::otaTaskHandle = nullptr;

void Ota::setup() {

  ArduinoOTA.setHostname("erou");

  ArduinoOTA
      .onStart([]() {
        vTaskPrioritySet(otaTaskHandle, 18);
        if (ArduinoOTA.getCommand() == U_FLASH) {
          debugI(logtags::ota, "Start updating via flash");
        } else {
          debugI(logtags::ota, "Start updating via spiffs");
        }
      })
      .onEnd([]() {
        debugI(logtags::ota, "\n\nEnd");
      })
      .onProgress([](unsigned int progress, unsigned int total) {
        // I don't like how it formats otherwise
        debugI(logtags::ota, "Progress: %u%%\r\033[1A'", (progress / (total / 100)));
      })
      .onError([](ota_error_t error) {
        debugI(logtags::ota, "\n\nError[%u]: %s ", error, Update.errorString());
        switch (error) {
          case OTA_AUTH_ERROR: debugI(logtags::ota, "Auth Failed");
            break;
          case OTA_BEGIN_ERROR: debugI(logtags::ota, "Begin Failed");
            break;
          case OTA_CONNECT_ERROR: debugI(logtags::ota, "Connect Failed");
            break;
          case OTA_RECEIVE_ERROR: debugI(logtags::ota, "Receive Failed");
            break;
          case OTA_END_ERROR: debugI(logtags::ota, "End Failed");
            break;
          default: debugI(logtags::ota, "Unknown Failture");
            break;
        }
      });

  ArduinoOTA.begin();

  debugI(logtags::ota, "Ready");
  debugI(logtags::ota, "IP address: %s", WiFi.localIP().toString().c_str());

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