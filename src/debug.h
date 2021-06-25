#pragma once
#include <WebSocketsServer.h>
#include <concurrentqueue.h>
#include <freertos/FreeRTOS.h>
#include <freertos/ringbuf.h>
#include <freertos/task.h>

#include <list>
#include <sstream>
#include <string>
#include <vector>

#include "config.h"
#define FMT_FORMAT_PROVIDE_PRINTF
#include "fmt/format.h"
#define debugV(fmt, ...)                                         \
  DebugInstance.printV("(%s)(%s)(C%d) " fmt, __FILE__, __func__, \
                       xPortGetCoreID(), ##__VA_ARGS__)
#define debugI(fmt, ...)                                                  \
  DebugInstance.printI("\033[1;32m(%s)(%s)(C%d) \033[1;0m" fmt, __FILE__, \
                       __func__, xPortGetCoreID(), ##__VA_ARGS__)
#define debugE(fmt, ...)                                                  \
  DebugInstance.printE("\033[1;31m(%s)(%s)(C%d) \033[1;0m" fmt, __FILE__, \
                       __func__, xPortGetCoreID(), ##__VA_ARGS__)

enum DebugLevel { VERBOSE, INFO, ERROR };

class Debug {
 private:
  struct DebugMessage {
    DebugLevel level;
    std::string content;
  };
  struct DebugCommand {
    std::string name;
    std::function<void(std::stringstream)> run;
  };
  TaskHandle_t messageLoopHandle;
  void messageBroadcastTask();
  inline static void messageBroadcastTaskWrapper(void *);

  std::vector<DebugCommand> commands;
  moodycamel::ConcurrentQueue<DebugMessage> messageQueue;
  void handleWsEvent(uint8_t num, WStype_t type, uint8_t *payload,
                     size_t length);
  SemaphoreHandle_t messageAcessSemaphore;
  WebSocketsServer webSocket = WebSocketsServer(config::debug::wsDebugPort);

 public:
  DebugLevel loggingLevel = DebugLevel::VERBOSE;
  void registerCommand(std::string name,
                       std::function<void(std::stringstream)> run);
  void printMessage(DebugLevel level, fmt::CStringRef format,
                    fmt::ArgList args);
  FMT_VARIADIC(void, printMessage, fmt::CStringRef)

  void printV(fmt::CStringRef format, fmt::ArgList args) {
    printMessage(DebugLevel::VERBOSE, format, args);
  };
  FMT_VARIADIC(void, printV, fmt::CStringRef)

  void printI(fmt::CStringRef format, fmt::ArgList args) {
    printMessage(DebugLevel::INFO, format, args);
  };
  FMT_VARIADIC(void, printI, fmt::CStringRef)

  void printE(fmt::CStringRef format, fmt::ArgList args) {
    printMessage(DebugLevel::ERROR, format, args);
  };
  FMT_VARIADIC(void, printE, fmt::CStringRef)

  Debug();
  void loop();
  void setup();
};

extern Debug DebugInstance;