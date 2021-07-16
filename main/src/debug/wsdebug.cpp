#include "wsdebug.h"
#include <deque>
#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>
#include <new>
#include "fmt/format.h"
#include "fmt/printf.h"
#include <functional>
#include "free_rtos_util.h"

Debug debugInstance = Debug();

//ESP-IDF callback for debug statements
int esp_apptrace_vprintf(const char *fmt, va_list ap);

void Debug::setup(AsyncWebServer &server) {
  server.addHandler(&ws);
  esp_log_set_vprintf(esp_apptrace_vprintf);

  ws.onEvent(std::bind(&Debug::handleWsEvent,
                       this,
                       std::placeholders::_1,
                       std::placeholders::_2,
                       std::placeholders::_3,
                       std::placeholders::_4,
                       std::placeholders::_5,
                       std::placeholders::_6));

}

Debug::Debug() {

  messageQueue = xQueueCreate(maxMessagesInQueue, sizeof(DebugMessage &));
  commandQueue = xQueueCreate(maxMessagesInQueue, sizeof(std::string));

  xTaskCreate(toFreeRtosTask(Debug, messageBroadcastTask),
              "Websocket Debug Task",
              config::debug::wsTaskStack,
              this,
              config::debug::wsTaskPriority,
              &messageLoopHandle
  );
  xTaskCreate(toFreeRtosTask(Debug, commandRunnerTask),
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
  auto *message = new DebugMessage{
      level,
      formattedString,
  };
  xQueueSend(messageQueue, &message, pdMS_TO_TICKS(5));

}

[[noreturn]] void Debug::commandRunnerTask() {
  if (commandQueue == nullptr) {
    debugE("Attempted to start command task without queue");
  }
  for (;;) {
    std::string *commandStringAddress;
    if (xQueueReceive(commandQueue, &commandStringAddress, portMAX_DELAY)) {

      auto commandString = std::unique_ptr<std::string>(commandStringAddress);
      bool commandRan = false;
      for (const DebugCommand &command : registeredCommands) {
        if (commandString->rfind(command.name, 0) != 0) {
          continue;
        }

        if (commandString->length() > command.name.length()) {
          const std::string args = commandString->substr(command.name.length());
          debugI("Matched Command `%s` with args `%s`", command.name, args);
          command.run(&args);
        } else {
          const std::string empty;
          debugI("Matched Command `%s`", command.name);
          command.run(&empty);
        }

        commandRan = true;
        break;
      }
      if (!commandRan) {
        debugE("Unknown Command: %s", *commandString);
      }
    }
  }
}

bool isInvalidChar(int c) {
  return !(c >= 0 && c < 128);
}

void stripUnicode(std::string &str) {
  str.erase(remove_if(str.begin(), str.end(), isInvalidChar), str.end());
}

[[noreturn]] void Debug::messageBroadcastTask() {

  DebugMessage *queueMessage;
  if (messageQueue == nullptr) {
    debugE("Attempted to start broadcast task without queue");
  }

  for (;;) {
    if (xQueueReceive(messageQueue, &queueMessage, portMAX_DELAY)) {
      auto message = std::unique_ptr<DebugMessage>(queueMessage);

      Serial.println(message->content.c_str());
      Serial.flush();

      if (message->level >= loggingLevel) {
        stripUnicode(message->content);
        ws.textAll(message->content.c_str(), message->content.length());
      }
      delay(10);
    }
  }
}

bool startsWith(const char *str, const char *pre) {
  return strncmp(pre, str, strlen(pre)) == 0;
}

void Debug::registerCommand(std::string name,
                            std::function<void(const std::string *)> run) {
  DebugCommand newCommand = {std::move(name), std::move(run)};
  registeredCommands.push_back(newCommand);
}
void Debug::handleWsEvent(AsyncWebSocket *,
                          AsyncWebSocketClient *client,
                          AwsEventType type,
                          void *arg,
                          uint8_t *data,
                          size_t len) {

  uint32_t clientId = client->id();
  AwsFrameInfo *info;
  switch (type) {
    case WS_EVT_DISCONNECT: {
      Serial.printf("Debug Websocket Client Disconnected: [%u]\n", clientId);
      break;
    }
    case WS_EVT_CONNECT: {
      IPAddress ip = client->remoteIP();
      Serial.printf(
          "Debug Websocket Client Connected: [%u] %d.%d.%d.%d\n", clientId,
          ip[0], ip[1], ip[2], ip[3]);

      client->text("Erou Debug Session (built at " __TIME__" " __DATE__  " ): ");
    }
      break;
    case WS_EVT_DATA: {
      info = static_cast<AwsFrameInfo *>(arg);
      if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
        auto *payloadText = new std::string(reinterpret_cast<char *>(data), info->len);
        xQueueSend(commandQueue, &payloadText, pdMS_TO_TICKS(500));
      }
      break;
    }
    default:break;
  }
}

int esp_apptrace_vprintf(const char *fmt, va_list ap) {
  char espString[50];
  int written = vsprintf(espString, fmt, ap);
  debugESP("ESP Log: %s", espString);
  return written;
}