#pragma once
#include <emTypes.h>
#include <mongoose.h>
#include <functional>
#include "httpStatusCodes.h"
#include "ArduinoJson.h"
namespace em {
struct Response {
  mg_connection *connection;
  std::string_view url;
  bool isSent;

  void sendJson(StatusCode code, const std::function<void(JsonDocument & )> &builder);
  void sendText(StatusCode code, const char *text);
};
}