#include <wsDebugMacros.h>
#include "easymongoose.h"
#include "lwip/inet.h"
namespace em {

void EasyMongoose::handleEvent(mg_connection *connection, int event, void *eventData) {
  struct in_addr peerIp;
  peerIp.s_addr = connection->peer.ip;
  char *peerIpString = inet_ntoa(peerIp);
  switch (event) {
    case MG_EV_POLL: {
      break;
    }
    case MG_EV_ACCEPT: {
      debugI(logtags::network,"Accepted connection from peer: %s", peerIpString);
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
      handleHttpMessage(connection, static_cast<struct mg_http_message *>(eventData));
      return;

    }
    case MG_EV_WS_MSG: {
      handleWsMessage(connection, static_cast<struct mg_ws_message *>(eventData));
      break;
    }
    case MG_EV_CLOSE: {
      debugI(logtags::network, "Closed connection from peer: %s", peerIpString);
      for (auto &[definition, clients] : wsConnections) {
        if (std::find(clients.begin(), clients.end(), connection) != clients.end()) {
          debugV(logtags::network, "Matched route for WS client removal: %s", definition.route.data());
          clients.erase(std::remove(clients.begin(), clients.end(), connection), clients.end());
        }
      }
      break;
    }
    default: {
      debugE(logtags::network, "Unhandled http event %d", event);
      break;
    }
  }
}

void EasyMongoose::handleWsMessage(mg_connection *connection, mg_ws_message *message) {
  for (auto &[definition, clients] : wsConnections) {
    if (std::find(clients.begin(), clients.end(), connection) != clients.end()) {
      debugV(logtags::network, "Matched route for WS message sending: %s", definition.route.data());
      definition.handler({
                             .text = std::string_view(message->data.ptr, message->data.len),
                             .reply = [&connection](std::string_view message) {
                               mg_ws_send(connection, message.data(), message.size(), WEBSOCKET_OP_TEXT);
                             }
                         });
    }
  }

  mg_iobuf_delete(&connection->recv, connection->recv.len);
}

void EasyMongoose::handleHttpMessage(mg_connection *connection, mg_http_message *message) {
  em::Request request = {
      .connection = connection,
      .url = std::string_view(message->uri.ptr),
      .params = {},
      .body = message->body
  };

  em::Response reply = {
      .connection = request.connection,
      .url = request.url,
      .isSent = false
  };

  for (em::HttpRouteDefinition &definition : routes) {
    if (!mg_http_match_uri(message, definition.routeGlob.c_str())) {
      continue;
    }
    if (mg_vcasecmp(&message->method, em::method::toString(definition.method)) != 0) {
      debugI(logtags::network, "Matched route but wrong method: %s", definition.routeGlob.c_str());
      continue;
    }
    debugV(logtags::network, "Matched route: %s", definition.routeGlob.c_str());
    routeRequest(message, request, reply, definition);
    return;
  }
  for (auto &[definition, clients] : wsConnections) {
    if (!mg_http_match_uri(message, definition.route.data())) {
      continue;
    }
    if (mg_vcasecmp(&message->method, em::method::toString(definition.method)) != 0) {
      debugI(logtags::network, "Matched route but wrong method: %s", definition.route.data());
      continue;
    }
    debugV(logtags::network, "Matched route: %s", definition.route.data());
    clients.push_back(connection);
    return;
  }
  reply.sendText(em::NotFound, "Path not found");
}

}