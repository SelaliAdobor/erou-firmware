#pragma once

#include "WiFiServer.h"

#include "WiFi.h"
#include <concurrentqueue.h>
#include <freertos/FreeRTOS.h>
#include <freertos/ringbuf.h>
#include <freertos/task.h>

#include <list>
#include <sstream>
#include <string>
#include <vector>
#include <AsyncTCP.h>
#include "ESPAsyncWebServer.h"

#include "config_constants.h"
#include "freertos/stream_buffer.h"
#include "fmt/format.h"

#define debugV(fmt, ...)                                         \
  debugInstance.printV("(%-25s)(%-50s)(C%d) " fmt, __FILE__, __PRETTY_FUNCTION__, \
                       xPortGetCoreID(), ##__VA_ARGS__)
#define debugI(fmt, ...)                                                  \
  debugInstance.printI("\033[1;32m(%-25s)(%-50s)(C%d) \033[1;0m" fmt, __FILE__, \
                       __PRETTY_FUNCTION__, xPortGetCoreID(), ##__VA_ARGS__)
#define debugESP(fmt, ...)                                                  \
  debugInstance.printI("\033[1;33m(%-25s)(%-50s)(C%d) \033[1;0m" fmt, "esp", \
                       "esp", xPortGetCoreID(), ##__VA_ARGS__)
#define debugE(fmt, ...)                                                  \
  debugInstance.printE("\033[1;31m(%-25s)(%-50s)(C%d) \033[1;0m" fmt, __FILE__, \
                       __PRETTY_FUNCTION__, xPortGetCoreID(), ##__VA_ARGS__)

enum DebugLevel {
  Verbose, Info, Error
};

class Debug {
 private:
  struct DebugMessage {
    DebugLevel level;
    std::string content;
  };

  struct DebugCommand {
    std::string name;
    std::function<void(const std::string *)> run;
  };

  TaskHandle_t messageLoopHandle{};
  TaskHandle_t commandRunnerHandle{};
  [[noreturn]] void messageBroadcastTask();

  inline static void messageBroadcastTaskWrapper(void *);

  [[noreturn]] void commandRunnerTask();
  inline static void commandRunnerTaskWrapper(void *);
  std::vector<DebugCommand> commands;

  moodycamel::ConcurrentQueue<DebugMessage> messageQueue;
  moodycamel::ConcurrentQueue<std::string> commandQueue;

  void handleWsEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data,
                     size_t len);
  AsyncWebSocket ws = AsyncWebSocket(config::debug::wsDebugEndpoint);

 public:
  DebugLevel loggingLevel = DebugLevel::Verbose;
  void registerCommand(std::string name,
                       std::function<void(const std::string *)> run);
  void printMessage(DebugLevel level, fmt::CStringRef format,
                    fmt::ArgList args);
  FMT_VARIADIC(void, printMessage, fmt::CStringRef)

  void printV(fmt::CStringRef format, fmt::ArgList args) {
    printMessage(DebugLevel::Verbose, std::move(format), std::move(args));
  };
  FMT_VARIADIC(void, printV, fmt::CStringRef)

  void printI(fmt::CStringRef format, fmt::ArgList args) {
    printMessage(DebugLevel::Info, std::move(format), std::move(args));
  };
  FMT_VARIADIC(void, printI, fmt::CStringRef)

  void printE(fmt::CStringRef format, fmt::ArgList args) {
    printMessage(DebugLevel::Error, std::move(format), std::move(args));
  };
  FMT_VARIADIC(void, printE, fmt::CStringRef)

  Debug();
  void setup(AsyncWebServer &server);
};

extern Debug debugInstance;
