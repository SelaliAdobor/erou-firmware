#pragma once

#include <list>
#include "config.h"

#include <WebSocketsServer.h>
#define FMT_FORMAT_PROVIDE_PRINTF
#include "fmt/format.h"
#define debugV(fmt, ...) DebugInstance.printV("(%s)(C%d) " fmt, __func__, xPortGetCoreID(), ##__VA_ARGS__)
#define debugI(fmt, ...) DebugInstance.printI("(%s)(C%d) " fmt, __func__, xPortGetCoreID(), ##__VA_ARGS__)
#define debugE(fmt, ...) DebugInstance.printE("(%s)(C%d) " fmt, __func__, xPortGetCoreID(), ##__VA_ARGS__)

enum DebugLevel
{
    VERBOSE,
    INFO,
    ERROR
};

class Debug
{
private:
    struct DebugMessage
    {
        DebugLevel level;
        std::string content;
    };
    void messageBroadcastTask();
    inline static void messageBroadcastTaskWrapper(void *);

    int maxMessageCount = 1000;
    std::list<DebugMessage> messages;
    void handleWsEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length);
    SemaphoreHandle_t messageAcessSemaphore;
    TaskHandle_t messageLoopHandle;
    WebSocketsServer webSocket = WebSocketsServer(config::debug::wsDebugPort);

public:
    DebugLevel loggingLevel = DebugLevel::VERBOSE;

    void printMessage(DebugLevel level, fmt::CStringRef format, fmt::ArgList args);
    FMT_VARIADIC(void, printMessage, fmt::CStringRef)

    void printV(fmt::CStringRef format, fmt::ArgList args) { printMessage(DebugLevel::VERBOSE, format, args); };
    FMT_VARIADIC(void, printV, fmt::CStringRef)

    void printI(fmt::CStringRef format, fmt::ArgList args) { printMessage(DebugLevel::INFO, format, args); };
    FMT_VARIADIC(void, printI, fmt::CStringRef)

    void printE(fmt::CStringRef format, fmt::ArgList args) { printMessage(DebugLevel::ERROR, format, args); };
    FMT_VARIADIC(void, printE, fmt::CStringRef)

    Debug();
    void loop();
    void setup();
};

extern Debug DebugInstance;
