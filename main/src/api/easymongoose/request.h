#pragma once
#include "etl/unordered_map.h"
#include "mongoose.h"
#include <string_view>
#include "ArduinoJson.h"
#include "etl_types.h"
namespace em {
using RouteParamsMap = etl::unordered_map<ShortString, std::string_view, 10>;

struct Request {
  static DynamicJsonDocument json;

  static Request*lastParsed;

  mg_connection *connection;
  std::string_view url;
  RouteParamsMap params;
  mg_str body;

  template<class T>
  std::pair<DeserializationError, T> parseJson() {
    if(lastParsed == this){
      return json.as<T>();
    }
    json.clear();
    return std::make_pair(deserializeJson(json, body.ptr, body.len), json.as<T>());
  }
};

}