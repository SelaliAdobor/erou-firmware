#include "ota.h"

#include "debug.h"
#include "ota_credentials.h"
Ota::Ota(TaskHandle_t handle) : secondaryLoopHandle(handle) {}

void Ota::setup() {
  TaskHandle_t loopHandle = secondaryLoopHandle;

  // Port defaults to 3232
  // ArduinoOTA.setPort(3232);

  ArduinoOTA.setHostname("erou");

  // No authentication by default
  // ArduinoOTA.setPassword("admin");

  // Password can be set with it's md5 value as well
  // MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
  // ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");

  ArduinoOTA
      .onStart([loopHandle]() {
        vTaskSuspend(loopHandle);
        if (ArduinoOTA.getCommand() == U_FLASH) {
          debugI("Start updating via flash");
        } else {
          debugI("Start updating via spiffs");
        }
      })
      .onEnd([loopHandle]() {
        vTaskResume(loopHandle);
        debugI("\nEnd");
      })
      .onProgress([](unsigned int progress, unsigned int total) {
        // I don't like how it formats otherwise
        debugI("Progress: %u%%\r", (progress / (total / 100)));
      })
      .onError([loopHandle](ota_error_t error) {
        vTaskResume(loopHandle);
        debugI("Error[%u]: ", error);
        switch (error) {
          case OTA_AUTH_ERROR:
            debugI("Auth Failed");
            break;
          case OTA_BEGIN_ERROR:
            debugI("Begin Failed");
            break;
          case OTA_CONNECT_ERROR:
            debugI("Connect Failed");
            break;
          case OTA_RECEIVE_ERROR:
            debugI("Receive Failed");
            break;
          case OTA_END_ERROR:
            debugI("End Failed");
            break;
          default:
            debugI("Unknown Failture");
            break;
        }
      });

  ArduinoOTA.begin();

  debugI("Ready");
  debugI("IP address: %s", WiFi.localIP().toString().c_str());
}

void Ota::loop() { ArduinoOTA.handle(); }