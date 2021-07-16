#pragma once
#include <containerManager.h>
#include "ESPAsyncWebServer.h"
class Api {
 private:
  void addContainer(AsyncWebServerRequest *request);
  void getContainer(AsyncWebServerRequest *request);
  ContainerManager *containerManager;
 public:
  Api(ContainerManager *container_manager) : containerManager(container_manager) {}
  void setup(AsyncWebServer server) {
    server.on("/containers",
              HTTP_POST,
              std::bind(&Api::addContainer, this, std::placeholders::_1));

    server.on("/containers/*",
              HTTP_GET,
              std::bind(&Api::getContainer, this, std::placeholders::_1));
  }
};
