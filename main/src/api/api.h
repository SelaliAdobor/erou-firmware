#pragma once
#include <containerManager.h>
#include "ESPAsyncWebServer.h"
#include "mongoose.h"
#include "etl/unordered_map.h"
#include "string_view"
#include "ArduinoJson.h"
#include "wsDebugMacros.h"
#include "wsdebug.h"

enum HttpMethod {
  Get,
  Post,
  Put,
};

enum HttpStatusCodes {
  NotFound = 404,
  Ok = 200,
  InternalError = 500,
  BadRequest = 400,
};
inline const char *methodToString(HttpMethod method) {
  switch (method) {
    case Get: return "GET";
    case Post: return "POST";
    case Put: return "PUT";
    default: return "";
  }
}
namespace httpHeaders {
static constexpr const char *jsonContentType = "Content-Type: text/json\r\n";
static constexpr const char *textContentType = "Content-Type: text/text\r\n";
}
using RouteParamsMap = etl::unordered_map<ShortString, std::string_view, 10>;
using RouteParamNamesMap = std::unordered_map<int, ShortString>;

struct HttpReply {
  mg_connection *connection;
  std::string_view url;
  std::string_view clientIp;
  bool isSent;
  void sendJson(HttpStatusCodes code, const std::function<void(JsonDocument &)> &builder) {
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
    mg_http_reply(connection, code, httpHeaders::jsonContentType, buffer.get());
    debugE("Sending Json: %s %d", buffer.get(), documentSize);
    isSent = true;
  }

  void sendText(HttpStatusCodes code, const char *text) {
    mg_http_reply(connection, code, httpHeaders::textContentType, text);
    isSent = true;
  }
};
struct WsMessage {
  std::string_view text;
  std::function<void(std::string_view)> reply;
};
using WsHandler = std::function<void(WsMessage&)>;
struct WsRoute {
  std::string_view route;
  HttpMethod method;
  WsHandler handler;
};
struct HttpRequest {
  mg_connection *connection;
  std::string_view url;
  std::string_view clientIp;
  RouteParamsMap params;
  mg_str body;
};
using HttpHandlerFunc = std::function<void(HttpRequest &, HttpReply &)>;
struct HttpRoute {
  std::string_view route;
  std::string routeGlob;
  HttpMethod method;
  RouteParamNamesMap routeParamNames;
  HttpHandlerFunc handler;
};
namespace etl {
template <>
struct hash<mg_connection>
{
  std::size_t operator()(const mg_connection& k) const
  {
    return (std::hash<uint64_t>()(k.id));
  }
};template <>
struct hash<WsRoute>
{
  std::size_t operator()(const WsRoute& k) const
  {
    return ((std::hash<std::string_view>()(k.route))
        ^ (hash<int>()(k.method) << 1)) >> 1;
  }
};

}
using WsConnectionMap = etl::unordered_map<mg_connection, WsRoute, 10>;
class Api {
 private:
  static constexpr size_t maxQueryParams = 20;
  void addContainer(HttpRequest &req, HttpReply &res);
  void getContainer(HttpRequest &req, HttpReply &res);
  void addDispense(HttpRequest &req, HttpReply &res);
  ContainerManager *containerManager;
  struct mg_mgr manager;
  [[noreturn]] void webserverTask();
  etl::vector<HttpRoute, 10> routes;
  etl::unordered_map<WsRoute, etl::vector<mg_connection* , 10>, 10> wsConnections;
  WsConnectionMap wsIdsToRoutes;
 public:
  explicit Api(ContainerManager *container_manager) : containerManager(container_manager) {}
  void setup();
  void handleEvent(mg_connection *connection, int event, void *eventData);
  void registerRoute(std::string_view route, HttpMethod method, HttpHandlerFunc handler);
  void registerWsRoute(std::string_view route, HttpMethod method, WsHandler handler);
  void sendAllWs(std::string_view route, std::string_view message);
  void routeRequest(mg_http_message *message, HttpRequest &request, HttpReply &reply, HttpRoute &route);
};

