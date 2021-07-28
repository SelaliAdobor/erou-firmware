#pragma once

enum class DebugLevel {
  Disabled, Verbose, Info, Error
};

namespace logtags {
static constexpr DebugLevel network = DebugLevel::Verbose;
static constexpr DebugLevel esp = DebugLevel::Verbose;
static constexpr DebugLevel dispense = DebugLevel::Verbose;
static constexpr DebugLevel motion = DebugLevel::Verbose;
static constexpr DebugLevel ota = DebugLevel::Verbose;
static constexpr DebugLevel storage = DebugLevel::Verbose;
static constexpr DebugLevel setup = DebugLevel::Verbose;
static constexpr DebugLevel debug = DebugLevel::Verbose;


}
static constexpr DebugLevel compileDebugLevel = DebugLevel::Verbose;


#define debugV(tag, fmt, ...)\
  if constexpr(tag >= compileDebugLevel){debugInstance.printMessage(DebugLevel::Verbose,"(%-25s)(%d)(%-10s)(C%d) " fmt, __FILE__, __LINE__ , __PRETTY_FUNCTION__, \
                       xPortGetCoreID(), ##__VA_ARGS__);}
#define debugI(tag, fmt, ...)\
  if constexpr(tag >= compileDebugLevel){debugInstance.printMessage(DebugLevel::Info,"\033[1;32m(%-25s)(%d)(%-10s)(C%d) \033[1;0m" fmt, __FILE__,__LINE__, \
                       __PRETTY_FUNCTION__, xPortGetCoreID(), ##__VA_ARGS__);}
#define debugESP(fmt, ...)\
  if constexpr(logtags::esp >= compileDebugLevel){ debugInstance.printMessage(DebugLevel::Info,"\043[1;33m(%-25s)(%-50s)(C%d) \033[1;0m" fmt, "esp",\
                       "esp", xPortGetCoreID(), ##__VA_ARGS__);}
#define debugE(tag, fmt, ...)\
  if constexpr(tag >= compileDebugLevel){ debugInstance.printMessage(DebugLevel::Error, "\033[1;31m(%-25s)(%d)(%-10s)(C%d) \033[1;0m" fmt, __FILE__,__LINE__, \
                       __PRETTY_FUNCTION__, xPortGetCoreID(), ##__VA_ARGS__);}
/*
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
*/
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

#include "config_constants.h"
#include "freertos/stream_buffer.h"
#include "debugCommand.h"

class Debug {
 private:
  const int maxMessagesInQueue = 50;
  QueueHandle_t messageQueue = nullptr;
  QueueHandle_t commandQueue = nullptr;

  struct DebugMessage {
    DebugLevel level;
    char *content;
  };

  TaskHandle_t messageLoopHandle{};
  TaskHandle_t commandRunnerHandle{};

  [[noreturn]] void messageBroadcastTask();

  [[noreturn]] void commandRunnerTask();

  struct mg_connection *connection;
 public:
  Debug();

  DebugLevel loggingLevel = DebugLevel::Verbose;

  void printMessage(DebugLevel level, const char *format, ...)
  __attribute__((__format__ (__printf__, 3, 4))) {
    char* content;

    va_list ap;
    va_start(ap, format);
    vasprintf(&content, format, ap);
    va_end(ap);
    auto *message = new DebugMessage{
        .level = level,
        .content = content,
    };
    Serial.println(content);
    xQueueSend(messageQueue, &message, pdMS_TO_TICKS(5));
  }

  void setup();

  static constexpr const char *debugEndpoint = "/debug";

};

extern Debug debugInstance;
