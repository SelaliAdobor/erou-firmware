#pragma once
#include <containerManager.h>
#include "ESPAsyncWebServer.h"
class Api {
 private:
  void addContainer(AsyncWebServerRequest *request);
  void getContainer(AsyncWebServerRequest *request);
  void addDispense(AsyncWebServerRequest *request);
  ContainerManager *containerManager;
 public:
  explicit Api(ContainerManager *container_manager) : containerManager(container_manager) {}
  void setup(AsyncWebServer* server) ;
};
