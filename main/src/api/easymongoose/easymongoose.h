#pragma once

#include <etl/vector.h>
#include "optional"
#include "httpRouteDefinition.h"
#include "mongoose.h"
#include "httpStatusCodes.h"
#include "method.h"
#include "request.h"
#include "response.h"
#include "wsRouteDefinition.h"
#include "etl/unordered_map.h"
#include "emTypes.h"
namespace em {
#ifdef ETL_CONSTANT
using WsConnectionMap = etl::unordered_map<WsRouteDefinition, etl::vector<mg_connection *, 10>, 10>;
#else
using WsConnectionMap = stl::unordered_map<WsRouteDefinition, etl::vector<mg_connection *>>;
#endif
class EasyMongoose {

 private:
  friend inline bool isListening();
  static constexpr size_t maxQueryParams = 20;
  static constexpr size_t maxRoutes = 10;
  static constexpr size_t maxPort = 99999; //Warning: Must change setup buffer size simultaneously
  static constexpr size_t listenTaskSize = 4028;
  static constexpr size_t listenTaskPriority = 8;

  static TaskHandle_t listenTask;
  struct mg_mgr manager;

  etl::vector<HttpRouteDefinition, maxRoutes> routes;
  WsConnectionMap wsConnections;

  [[noreturn]] static void webserverTask(void *);
  void handleEvent(mg_connection *connection, int event, void *eventData);
  void routeRequest(mg_http_message *message,
                    Request &request,
                    Response &reply,
                    HttpRouteDefinition &definition);

  void handleHttpMessage(mg_connection *connection, mg_http_message *message);
  void handleWsMessage(mg_connection *connection, mg_ws_message *message);
 public:
  std::optional<std::string_view> setup(const std::string_view &localhost);
  std::optional<std::string_view> setup(const int port) {
    if (port > 99999) {
      return "Max port exceeded";
    }
    //Setup buffer, based on max pot "0.0.0.0:99999"
    char hostname[15];
    sprintf(hostname, "0.0.0.0:%d", port);
    return setup(hostname);
  }
  void registerRoute(std::string_view route, method::Value method, HttpRouteHandler handler);
  void registerWsRoute(std::string_view route, method::Value method, WsHandler handler);
  void sendAllWs(std::string_view route, std::string_view message);
};

static EasyMongoose instance;
inline std::optional<std::string_view> setup(const std::string_view &localhost) {
  return instance.setup(localhost);
}
inline std::optional<std::string_view> setup(const int port) {
  return instance.setup(port);
}
inline void registerRoute(const std::string_view route, method::Value method, const HttpRouteHandler &handler) {
  return instance.registerRoute(route, method, handler);
}
inline void registerWsRoute(const std::string_view &route, method::Value method, const WsHandler &handler) {
  return instance.registerWsRoute(route, method, handler);
}
inline void sendAllWs(const std::string_view &route, const std::string_view &message) {
  return instance.sendAllWs(route, message);
}
inline bool isListening() {
  return EasyMongoose::listenTask != nullptr;
}
}