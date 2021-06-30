#include "debug.h"
#include <deque>
#include <freertos/FreeRTOS.h>

#include "fmt/format.h"
#include "fmt/printf.h"
#include <functional>
#include <memory>

Debug debugInstance = Debug();

void Debug::setup(AsyncWebServer &server) {
  server.addHandler(&ws);
  using namespace std::placeholders;  /* NOLINT(google-build-using-namespace)
                                           (If someone redefines _X they deserve the clash.)*/
  ws.onEvent(std::bind(&Debug::handleWsEvent,
                       this, std::placeholders::_1, _2, _3, _4, _5, _6));

}

Debug::Debug() {

  xTaskCreate(messageBroadcastTaskWrapper,
              "Websocket Debug Task",
              config::debug::wsTaskStack,
              this,
              config::debug::wsTaskPriority,
              &messageLoopHandle
  );
  xTaskCreate(commandRunnerTaskWrapper,
              "Command Runner Debug Task",
              config::debug::wsTaskStack,
              this,
              config::debug::wsTaskPriority,
              &commandRunnerHandle
  );
  registerCommand("level ", [this](const std::string *args) {
    int newLevel = 0;
    if (sscanf(args->c_str(), "%d", &newLevel) != 1) {
      debugE("Failed to parse args %s", *args);

    }
    loggingLevel = DebugLevel(newLevel);
    debugI("Setting log level to %d", loggingLevel);
  });
}

void Debug::printMessage(DebugLevel level, fmt::CStringRef format,
                         fmt::ArgList args) {
  auto formattedString = std::string(fmt::sprintf(format, args));
  DebugMessage message;
  message.content = formattedString;
  message.level = level;

  messageQueue.enqueue(message);
  xTaskNotifyGive(messageLoopHandle);
}

void Debug::commandRunnerTaskWrapper(void *_this) {
  static_cast<Debug *>(_this)->commandRunnerTask();
}

[[noreturn]] void Debug::commandRunnerTask()  {
  for (;;) {
    ulTaskNotifyTake(pdTRUE, pdMS_TO_TICKS(2000));
    std::string commandString;
    while (commandQueue.try_dequeue(commandString)) {
      bool commandRan = false;
      for (const DebugCommand &command : commands) {
        if (commandString.rfind(command.name, 0) != 0){
          continue; //Command didn't match
        }

        const std::string args = commandString.substr(command.name.length());
        debugI("Matched Command with: %s", args);
        command.run(&args);
        commandRan = true;
        break;
      }
      if(!commandRan){
        debugE("Unknown Command: %s", commandString);
      }
    }
  }
}
void Debug::messageBroadcastTaskWrapper(void *_this) {
  static_cast<Debug *>(_this)->messageBroadcastTask();
}

[[noreturn]] void Debug::messageBroadcastTask() {
  for (;;) {
    ulTaskNotifyTake(pdTRUE, pdMS_TO_TICKS(2000));
    //Allow 15 seconds for debug connection before broadcasting regardless of WS connection
    if(ws.getClients().empty() && (millis() < 15000)){
      continue;
    }
    DebugMessage message;
    while (messageQueue.try_dequeue(message)) {
      Serial.println(message.content.c_str());
      if (message.level >= loggingLevel) {
        ws.textAll(message.content.c_str(), message.content.length());
        Serial.println(message.content.c_str());
      }
    }
  }
}

bool startsWith(const char *str, const char *pre) {
  return strncmp(pre, str, strlen(pre)) == 0;
}

void Debug::registerCommand(std::string name,
                            std::function<void(const std::string *)> run) {
  DebugCommand newCommand = {std::move(name), std::move(run)};
  commands.push_back(newCommand);
}

void Debug::handleWsEvent(AsyncWebSocket *,
                          AsyncWebSocketClient *client,
                          AwsEventType type,
                          void *arg,
                          uint8_t *payload,
                          size_t len) {
  std::string payloadText;
  uint32_t clientId = client->id();
  AwsFrameInfo *info;
  switch (type) {
    case WS_EVT_DISCONNECT:Serial.printf("Debug Websocket Client Disconnected: [%u]\n", clientId);
      break;
    case WS_EVT_CONNECT: {
      IPAddress ip = client->remoteIP();
      Serial.printf(
          "Debug Websocket Client Connected: [%u] %d.%d.%d.%d\n", clientId,
          ip[0], ip[1], ip[2], ip[3]);

      client->text("Erou Debug Session (built at " __TIME__" " __DATE__  " ): ");
      xTaskNotifyGive(messageLoopHandle);
    }
      break;
    case WS_EVT_DATA:info = static_cast<AwsFrameInfo *>(arg);

      if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
        payloadText = std::string(reinterpret_cast<char *>(payload), info->len);
        commandQueue.try_enqueue(payloadText);
        xTaskNotifyGive(commandRunnerHandle);
      }

      break;
    default:break;
  }
}
