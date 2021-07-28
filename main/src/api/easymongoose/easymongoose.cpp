

#include <yuarel.h>
#include "easymongoose.h"
#include "stringUtil.h"
#include "wsdebug.h"
namespace em {
EasyMongoose EasyMongoose::instance = EasyMongoose();
TaskHandle_t EasyMongoose::listenTask = nullptr;

std::optional<std::string_view> EasyMongoose::setup(const std::string_view &localhost) {
  if (listenTask != nullptr) {
    return "Already setup EasyMongoose, only one instance is currently supported";
  }
  mg_mgr_init(&manager);

  struct mg_connection *serverConnection = mg_http_listen(&manager, localhost.data(), [](
      struct mg_connection *connection, int event, void *eventData, void *fn_data) {
    static_cast<EasyMongoose *>(fn_data)->handleEvent(connection, event, eventData);
  }, this);

  if (!serverConnection) {
    return "Failed to start mongoose listen, is port in use?";
  }
  xTaskCreate(EasyMongoose::webserverTask,
              "Mongoose Listen Task",
              listenTaskSize,
              &manager,
              listenTaskPriority, &listenTask);
  return {};
}

void EasyMongoose::webserverTask(void *manager) {
  for (;;) {
    mg_mgr_poll(static_cast<mg_mgr *>(manager), 1000);
  }
}

void EasyMongoose::registerRoute(std::string_view route,
                                 em::method::Value method,
                                 const em::HttpRouteHandler handler) {
  auto paramCount = std::count(route.begin(), route.end(), ':');

  if (paramCount == 0) {
    routes.push_back({
                         .route = route,
                         .routeGlob= std::string(route),
                         .method = method,
                         .routeParamNames = em::RouteParamNamesMap(),
                         .handler =  std::move(handler)
                     });
    debugI(logtags::network, "Found no params %s", route.data());
    return;
  }

  size_t currentCharacter = 0;
  int currentPart = 0;

  em::RouteParamNamesMap foundParams;
  auto routeGlob = std::string(route);
  while (currentCharacter < route.length()) {
    if (route[currentCharacter] == '/') {
      currentPart++;
    } else if ((route[currentCharacter] == ':') && (currentPart >= 0)) {
      size_t nextSlash = route.find('/', currentCharacter) - 1;
      std::string_view paramName = route.substr(currentCharacter + 1, nextSlash - currentCharacter);
      foundParams.insert({currentPart, ShortString(paramName.begin(), paramName.end())});
      routeGlob.replace(currentCharacter, nextSlash - currentCharacter,"*");
      currentCharacter = nextSlash;
      continue;
    }
    currentCharacter++;
  }
  debugI(logtags::network, "Found %d params for %s with glob %s", foundParams.size(), route.data(), routeGlob.c_str());
  routes.push_back({
                       .route = route,
                       .routeGlob= routeGlob,
                       .method = method,
                       .routeParamNames = foundParams,
                       .handler =  std::move(handler),
                   });
}

void EasyMongoose::registerWsRoute(std::string_view route,
                                   em::method::Value method,
                                   em::WsHandler handler) {
  em::WsRouteDefinition definition = {
      .route = route,
      .method = method,
      .handler= std::move(handler)
  };

  wsConnections.insert({definition, {}});
}

void EasyMongoose::sendAllWs(const std::string_view route, std::string_view message) {
  for (auto &[definition, clients] : wsConnections) {
    if (definition.route.compare(route) == 0) {
      for (auto &client : clients) {
        mg_ws_send(client, message.data(), message.length(), WEBSOCKET_OP_TEXT);
      }
      return;
    }
  }
  debugE(logtags::network, "Send all found no matching routes: %s", route.data());
}

void EasyMongoose::routeRequest(mg_http_message *message,
                                em::Request &request,
                                em::Response &reply,
                                em::HttpRouteDefinition &definition) {

  for (auto&[paramIndex, paramName] : definition.routeParamNames) {
    auto uri = std::string_view(message->uri.ptr, message->uri.len);
    size_t start = findNthOccurrence(uri, '/', paramIndex) + 1;
    size_t end = findNthOccurrence(uri, '/', paramIndex + 1);
    std::string_view paramValue = uri.substr(start, end - start);
    request.params.insert({paramName, paramValue});
  }

  static yuarel_param params[maxQueryParams]; //Precludes use of concurrent handlers
  if (message->query.ptr) {
    auto queryString = std::unique_ptr<char>(strndup(message->query.ptr, message->query.len));
    debugI(logtags::network, "Query: %s", queryString.get());
    int queryParamCount = yuarel_parse_query(queryString.get(), '&', params, maxQueryParams);
    for (int i = 0; i < queryParamCount; i++) {
      request.params.insert({ShortString(params[i].key), std::string_view(params[i].val)});
    }
  }

  for (auto&[name, value] : request.params) {
    if (name.is_truncated()) {
      //TODO: Reply with error code;
      debugE(logtags::network, "Truncated param name: %d %s", name.is_truncated(), name.c_str());
    }
  }

  definition.handler(request, reply);
  if (!reply.isSent) {
    reply.sendText(em::InternalError, "Internal Server Error (Reply not sent)");
  }
}
}