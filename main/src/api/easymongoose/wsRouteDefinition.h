#pragma once

#include <tuple>
#include <functional>
#include "wsMessage.h"
#include "method.h"
#include "etl/hash.h"
namespace em {
using WsHandler = std::function<void(const WsMessage &)>;

struct WsRouteDefinition {
  std::string_view route;
  method::Value method;
  WsHandler handler;
  bool operator==(const WsRouteDefinition &rhs) const {
    return std::tie(route, method) == std::tie(rhs.route, rhs.method);
  }
  bool operator!=(const WsRouteDefinition &rhs) const {
    return !(rhs == *this);
  }
};
}

namespace etl {
template<>
struct hash<em::WsRouteDefinition> {
  std::size_t operator()(const em::WsRouteDefinition &route) const {
    std::size_t value = (std::hash<std::string_view>()(route.route));
    return value ^ ((std::hash<int>()(static_cast<int>(route.method))) + 0x9e3779b9 + (value << 6) + (value >> 2));
  }
};
}
