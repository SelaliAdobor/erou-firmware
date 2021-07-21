#include <container.h>
#include "api.h"

void Api::setup(AsyncWebServer* server) {

  server->on("/containers",
            HTTP_POST,
            [this](auto && PH1) { addContainer(std::forward<decltype(PH1)>(PH1)); });

  server->on("/containers/*",
            HTTP_GET,
            [this](auto && PH1) { getContainer(std::forward<decltype(PH1)>(PH1)); });

}