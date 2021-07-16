#include <container.h>
#include "api.h"

void Api::setup(AsyncWebServer server) {

  server.on("/containers",
            HTTP_POST,
            std::bind(&Api::addContainer, this, std::placeholders::_1));

  server.on("/containers/*",
            HTTP_GET,
            std::bind(&Api::getContainer, this, std::placeholders::_1));

}