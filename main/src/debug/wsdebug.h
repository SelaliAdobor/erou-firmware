#pragma once

#define debugV(fmt, ...)\
  debugInstance.printMessage(DebugLevel::Verbose,"(%-25s)(%d)(%-10s)(C%d) " fmt, __FILE__, __LINE__ , __PRETTY_FUNCTION__, \
                       xPortGetCoreID(), ##__VA_ARGS__)
#define debugI(fmt, ...)\
  debugInstance.printMessage(DebugLevel::Info,"\033[1;32m(%-25s)(%d)(%-10s)(C%d) \033[1;0m" fmt, __FILE__,__LINE__, \
                       __PRETTY_FUNCTION__, xPortGetCoreID(), ##__VA_ARGS__)
#define debugESP(fmt, ...)\
  debugInstance.printMessage(DebugLevel::Info,"\033[1;33m(%-25s)(%-50s)(C%d) \033[1;0m" fmt, "esp",\
                       "esp", xPortGetCoreID(), ##__VA_ARGS__)
#define debugE(fmt, ...)\
  debugInstance.printMessage(DebugLevel::Error, "\033[1;31m(%-25s)(%d)(%-10s)(C%d) \033[1;0m" fmt, __FILE__,__LINE__, \
                       __PRETTY_FUNCTION__, xPortGetCoreID(), ##__VA_ARGS__)

#include "WiFiServer.h"

#include "WiFi.h"
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <functional>
#include <list>
#include <string>
#include <vector>
#include <AsyncTCP.h>
#include <etl_types.h>
#include <fmt/printf.h>
#include <api.h>

#include "config_constants.h"
#include "freertos/stream_buffer.h"
#include "fmt/format.h"
#include "debugCommand.h"
#include "mongoose.h"

enum DebugLevel {
  Verbose, Info, Error
};

class Debug {
 private:
  const int maxMessagesInQueue = 50;
  QueueHandle_t messageQueue = nullptr;
  QueueHandle_t commandQueue = nullptr;

  struct DebugMessage {
    DebugLevel level;
    LongString *content;
  };

  TaskHandle_t messageLoopHandle{};
  TaskHandle_t commandRunnerHandle{};

  [[noreturn]] void messageBroadcastTask();

  [[noreturn]] void commandRunnerTask();

  struct mg_connection *connection;
  Api* api;
 public:
  Debug();

  DebugLevel loggingLevel = DebugLevel::Verbose;

  template<typename... Args>
  void printMessage(DebugLevel level,const char *format,
                    const Args &...args) {
    auto *formattedString = new LongString(fmt::sprintf(format, args...).c_str());
    auto *message = new DebugMessage{
        .level = level,
        .content = formattedString,
    };
    Serial.println(formattedString->c_str());
    xQueueSend(messageQueue, &message, pdMS_TO_TICKS(5));
  }

  template<typename... Args>
  void printV(const char *format, const Args &...args) {
    printMessage(DebugLevel::Verbose, format, args...);
  };

  template<typename... Args>
  inline void printI(const char *format, const Args &...args) {
    printMessage(DebugLevel::Info, format, args...);
  };

  template<typename... Args>
  void printE(const char *format, const Args &...args) {
    printMessage(DebugLevel::Error, format, args...);
  };

  void setup(Api* api);

  static constexpr const char *debugEndpoint = "/debug";
  void handleWsMessage(struct mg_ws_message *message);

  void setConnection(struct mg_connection *conn) { this->connection = conn; }
  void clearConnection(struct mg_connection *conn) {
    if (this->connection == conn) {
      connection = nullptr;
    }
  }
};

extern Debug debugInstance;
