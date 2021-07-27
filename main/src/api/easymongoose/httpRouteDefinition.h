#pragma once
#include "etl_types.h"
#include "request.h"
#include "response.h"
#include "method.h"
namespace em{
using HttpRouteHandler = std::function<void(Request &, Response &)>;
using RouteParamNamesMap = std::unordered_map<int, ShortString>;

struct HttpRouteDefinition {
  std::string_view route;
  std::string routeGlob;
  method::Value method;
  RouteParamNamesMap routeParamNames;
  HttpRouteHandler handler;
};
}