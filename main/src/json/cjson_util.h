#pragma once
#include "cJSON.h"

struct SafePrintDeleter {
  void operator()(char *str) {
    free(str);
  }
};
inline std::unique_ptr<char, SafePrintDeleter> safePrint(cJSON *item) {
  return std::unique_ptr<char, SafePrintDeleter>(cJSON_Print(item));
}
inline void safePrint(const cJSON *item, std::unique_ptr<char, SafePrintDeleter> ptr) {
  return ptr.reset(cJSON_Print(item));
}
struct SafeParseDeleter {
  void operator()(cJSON *json) {
    cJSON_Delete(json);
  }
};
inline std::unique_ptr<cJSON, SafeParseDeleter> safeParse(const char *string) {
  return std::unique_ptr<cJSON, SafeParseDeleter>(cJSON_Parse(string));
}