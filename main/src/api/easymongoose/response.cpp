#include <memory>
#include "response.h"
#include "httpStatusCodes.h"
#include "functional"
#include "ArduinoJson.h"
#include "wsdebug.h"
#include "httpHeaders.h"
#include "wsdebug.h"
namespace em {

void Response::sendJson(StatusCode code, const std::function<void(JsonDocument & )> &builder) {
  if (isSent) {
    debugE(logtags::network, "Attempted to send multiple replies");
    return;
  }
  static DynamicJsonDocument replyDoc(512);
  replyDoc.clear();
  builder(replyDoc);
  if (replyDoc.overflowed()) {
    debugE(logtags::network, "Overflowed json");
    sendText(InternalError, "Json response overflowed");
    return;
  }
  auto documentSize = measureJson(replyDoc) + 1; //Account for null terminator
  auto buffer = std::unique_ptr<char>(new char[documentSize]);
  serializeJson(replyDoc, buffer.get(), documentSize);
  *(buffer.get() + (documentSize - 1)) = '\0';
  mg_http_reply(connection, code, headers::contentTypeJson, "%s", buffer.get());
  debugE(logtags::network, "Sending Json: %s %d", buffer.get(), documentSize);
  isSent = true;
}

void Response::sendText(StatusCode code, const char *text) {
  if (isSent) {
    debugE(logtags::network, "Attempted to send multiple replies");
    return;
  }
  mg_http_reply(connection, code, headers::contentTypeText, "%s", text);
  isSent = true;
}
void Response::sendMissingFields(const std::vector<const char *> &missingFields) {
  sendJson(em::BadRequest, [&](auto &reply) {
    auto replyFields = reply.createNestedArray("missingFields");
    for (const auto *field : missingFields) {
      debugV(logtags::network, "Missing field %s", field);
      replyFields.add(field);
    }
  });
}
}