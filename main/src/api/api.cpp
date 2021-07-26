#include <container.h>
#include "mongoose.h"
#include <free_rtos_util.h>
#include "api.h"
#include "requestUtil.h"
#include <arpa/inet.h>
#include "yuarel.h"
#include <utility>
#include <etl/unordered_map.h>
#include <regex>
#include <algorithm>
void Api::setup() {
  mg_mgr_init(&manager);

  struct mg_connection *serverConnection = mg_http_listen(&manager, config::network::hostname, [](
      struct mg_connection *connection, int event, void *eventData, void *fn_data) {
    static_cast<Api *>(fn_data)->handleEvent(connection, event, eventData);
  }, this);

  if (!serverConnection) {
    debugE("Failed to listen to port");
  }
  registerRoute("/container", Post, [=](auto &&...args) { return addContainer(args...); });
  registerRoute("/reset", Get, [=](HttpRequest, HttpReply res) {
    storedSettings.reset();
    res.sendText(Ok, "Reset stored settings");
  });
  registerRoute("/container/:id", Get, [=](auto &&...args) { return getContainer(args...); });
  xTaskCreate(toFreeRtosTask(Api, webserverTask), "Mongoose Listen Task", config::network::listenTaskSize,
              this, 4, nullptr);
}
void Api::webserverTask() {
  for (;;) {
    mg_mgr_poll(&manager, 1000);
  }
}
size_t findNthOccur(std::string_view str,
                    char ch, int N) {
  int occur = 0;

  // Loop to find the Nth
  // occurence of the character
  for (int i = 0; i < str.length(); i++) {
    if (str[i] == ch) {
      occur += 1;
    }
    if (occur == N)
      return i;
  }
  return str.length();
}
void Api::handleEvent(mg_connection *connection, int event, void *eventData) {
  struct in_addr peerIp;
  peerIp.s_addr = connection->peer.ip;
  char *peerIpString = inet_ntoa(peerIp);
  switch (event) {
    case MG_EV_POLL: {
      break;
    }
    case MG_EV_ACCEPT: {
      debugI("Accepted connection from peer: %s", peerIpString);
      break;
    }
    case MG_EV_READ: {
      //Use Serial to avoid echoing to debug socket
      Serial.printf("Read data from peer: %s\n", peerIpString);
      break;
    }
    case MG_EV_WRITE: {
      //Use Serial to avoid echoing to debug socket
      Serial.printf("Wrote data to peer: %s\n", peerIpString);
      break;
    }
    case MG_EV_HTTP_MSG : {
      auto *message = static_cast<struct mg_http_message *>(eventData);
      HttpRequest request = {
          .connection = connection,
          .url = std::string_view(message->uri.ptr),
          .clientIp = std::string_view(inet_ntoa(peerIp)),
          .params = {},
          .body = message->body
      };

      HttpReply reply = {
          .connection = request.connection,
          .url = request.url,
          .clientIp = request.clientIp,
          .isSent = false
      };

      for (HttpRoute &route : routes) {
        if (!mg_http_match_uri(message, route.routeGlob.c_str())) {
          continue;
        }
        if (mg_vcasecmp(&message->method, methodToString(route.method)) != 0) {
          debugI("Matched route but wrong method: %s", route.routeGlob);
          continue;
        }
        debugV("Matched route: %s", route.routeGlob);
        routeRequest(message, request, reply, route);
        return;
      }
      for (auto &[route, clients] : wsConnections) {
        if (!mg_http_match_uri(message, route.route.data())) {
          continue;
        }
        if (mg_vcasecmp(&message->method, methodToString(route.method)) != 0) {
          debugI("Matched route but wrong method: %s", route.route);
          continue;
        }
        debugV("Matched route: %s", route.route);
        clients.push_back(connection);
        return;
      }
      reply.sendText(NotFound, "Path not found");
      return;
    }
    case MG_EV_WS_MSG: {
      //For now all WS connections are WSDebug connections, in the future a tag may be needed
      auto *message = static_cast<struct mg_ws_message *>(eventData);

      for (auto &[route, clients] : wsConnections) {
        if (std::find(clients.begin(), clients.end(), connection) != clients.end()) {
          debugV("Matched route for WS message sending: %s", route.route);
          route.handler(
              {
                  .text = std::string_view(message->data.ptr, message->data.len),
                  .reply = [&connection](std::string_view message) {
                    mg_ws_send(connection, message.data(), message.size(), WEBSOCKET_OP_TEXT);
                  }
              }
          );
        }
      }

      mg_iobuf_delete(&connection->recv, connection->recv.len);
      break;
    }
    case MG_EV_CLOSE: {
      debugI("Closed connection from peer: %s", peerIpString);
      for (auto &[route, clients] : wsConnections) {
        if (std::find(clients.begin(), clients.end(), connection) != clients.end()) {
          debugV("Matched route for WS client removal: %s", route.route);
          clients.erase(std::remove(clients.begin(), clients.end(), connection), clients.end());
        }
      }
      break;
    }
    default: {
      debugE("Unhandled http event %d", event);
      break;
    }
  }
}
void Api::routeRequest(mg_http_message *message,
                       HttpRequest &request,
                       HttpReply &reply,
                       HttpRoute &route) {

  for (auto&[paramIndex, paramName] : route.routeParamNames) {
    auto uri = std::string_view(message->uri.ptr, message->uri.len);
    size_t start = findNthOccur(uri, '/', paramIndex) + 1;
    size_t end = findNthOccur(uri, '/', paramIndex + 1);
    std::string_view paramValue = uri.substr(start, end - start);
    request.params.insert({paramName, paramValue});
  }

  static yuarel_param params[maxQueryParams]; //Precludes use of concurrent handlers
  if (message->query.ptr) {
    auto queryString = std::unique_ptr<char>(strndup(message->query.ptr, message->query.len));
    debugI("Query: %s", queryString.get());
    int queryParamCount = yuarel_parse_query(queryString.get(), '&', params, maxQueryParams);
    for (int i = 0; i < queryParamCount; i++) {
      request.params.insert({ShortString(params[i].key), std::string_view(params[i].val)});
    }
  }

  for (auto&[name, value] : request.params) {
    if (name.is_truncated()) {
      //TODO: Reply with error code;
      debugE("Truncated param name: %d %s", name.is_truncated(), name.c_str());
    }
  }

  route.handler(request, reply);
  if (!reply.isSent) {
    reply.sendText(InternalError, "Internal Server Error (Reply not sent)");
  }
}
void Api::registerRoute(std::string_view route,
                        HttpMethod method,
                        const HttpHandlerFunc handler) {
  auto paramCount = std::count(route.begin(), route.end(), ':');

  if (paramCount == 0) {
    routes.push_back({
                         .route = route,
                         .routeGlob= std::string(route),
                         .method = method,
                         .routeParamNames = RouteParamNamesMap(),
                         .handler =  std::move(handler)
                     });
    debugI("Found no params %s", route);
    return;
  }

  size_t currentCharacter = 0;
  int currentPart = 0;
  const static std::regex paramRegex(":.*?(?:(?!/|$).)*");

  RouteParamNamesMap foundParams;

  while (currentCharacter < route.length()) {
    if (route[currentCharacter] == '/') {
      currentPart++;
    } else if ((route[currentCharacter] == ':') && (currentPart >= 0)) {
      size_t nextSlash = route.find('/', currentCharacter) - 1;
      std::string_view paramName = route.substr(currentCharacter + 1, nextSlash - currentCharacter);
      foundParams.insert({currentPart, ShortString(paramName.begin(), paramName.end())});
      currentCharacter = nextSlash;
      continue;
    }
    currentCharacter++;
  }
  routes.push_back({
                       .route = route,
                       .routeGlob= std::regex_replace(std::string(route), paramRegex, "*"),
                       .method = method,
                       .routeParamNames = foundParams,
                       .handler =  std::move(handler),
                   });

}
void Api::registerWsRoute(std::string_view route,
                          HttpMethod method,
                          WsHandler handler) {
  WsRoute wsRoute = {
      .route = route,
      .method = method,
      .handler= std::move(handler)
  };

  wsConnections.insert({wsRoute, {}});
}
void Api::sendAllWs(const std::string_view route, std::string_view message) {
  for (auto &[wsRoute, clients] : wsConnections) {
    if (wsRoute.route.compare(route) == 0) {
      debugV("Matched route for WS sendAll: %s", route);
      for (auto &client : clients) {
        mg_ws_send(client, message.data(), message.length(), WEBSOCKET_OP_TEXT);
      }
      return;
    }
  }
  debugE("Send all found no matching routes: %s", route.data());
}