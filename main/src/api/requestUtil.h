#pragma once
#include "ESPAsyncWebServer.h"
#include "cjson_util.h"
template<class T>
inline T requestParamAs(AsyncWebServerRequest *request, const char *param);

template<>
inline std::string requestParamAs<std::string>(AsyncWebServerRequest *request, const char *param) {
  return std::string(request->getParam(param, true)->value().c_str());
}

template<>
inline int requestParamAs<int>(AsyncWebServerRequest *request, const char *param) {
  return static_cast<int>(request->getParam(param, true)->value().toInt());
}

template<>
inline int64_t requestParamAs<int64_t>(AsyncWebServerRequest *request, const char *param) {
  return static_cast<int64_t>(request->getParam(param, true)->value().toInt());
}

inline bool sendErrorIfMissing(AsyncWebServerRequest *request, std::vector<const char *> params) {
  std::vector<const char *> missingParams = std::vector<const char *>(params.size());
  for (const char *param : params) {
    if (!request->hasParam(param, true)) {
      missingParams.push_back(param);
    }
  }

  if (!missingParams.empty()) {
    auto *response = cJSON_CreateObject();
    cJSON_AddBoolToObject(response, "error", 1);
    cJSON_AddStringToObject(response, "cause", "missing fields");

    auto *missingFields = cJSON_AddArrayToObject(response, "missingFields");

    for (const char *missingParam : params) {
      cJSON_AddItemToArray(missingFields, cJSON_CreateString(missingParam));
    }
    request->send(500, "text/json", safePrint(response).get());
    cJSON_Delete(response);
    return true;
  }
  return false;
}

inline void replyWithError(AsyncWebServerRequest *request, const char *error) {
  auto *response = cJSON_CreateObject();
  cJSON_AddBoolToObject(response, "error", 1);
  cJSON_AddStringToObject(response, "cause", error);

  request->send(500, "text/json", safePrint(response).get());

}

inline void replyWithJson(AsyncWebServerRequest *request, int code, cJSON *json) {
  request->send(code, "text/json", safePrint(json).get());
}