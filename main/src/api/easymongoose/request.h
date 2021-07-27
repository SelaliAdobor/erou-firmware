#pragma once
#include "etl/unordered_map.h"
#include "mongoose.h"
#include <string_view>
namespace em{
using RouteParamsMap = etl::unordered_map<ShortString, std::string_view, 10>;
struct Request {
  mg_connection *connection;
  std::string_view url;
  RouteParamsMap params;
  mg_str body;
};
}