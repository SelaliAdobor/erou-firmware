#include <memory>
#include "response.h"
#include "httpStatusCodes.h"
#include "functional"
#include "ArduinoJson.h"
#include "wsdebug.h"
#include "httpHeaders.h"

namespace em {

void Response::sendJson(StatusCode code, const std::function<void(JsonDocument & )> &builder) {
  static DynamicJsonDocument replyDoc(512);
  replyDoc.clear();
  builder(replyDoc);
  if (replyDoc.overflowed()) {
    sendText(InternalError, "Json response overflowed");
    return;
  }
  auto documentSize = measureJson(replyDoc) + 1; //Account for null terminator
  auto buffer = std::unique_ptr<char>(new char[documentSize]);
  serializeJson(replyDoc, buffer.get(), documentSize);
  mg_http_reply(connection, code, headers::contentTypeJson , buffer.get());
  debugE("Sending Json: %s %d", buffer.get(), documentSize);
  isSent = true;
}

void Response::sendText(StatusCode code, const char *text) {
  mg_http_reply(connection, code, headers::contentTypeText, text);
  isSent = true;
}
}