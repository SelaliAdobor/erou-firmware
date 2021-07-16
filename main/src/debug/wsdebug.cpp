#include "wsdebug.h"
#include <deque>
#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>
#include <new>
#include "fmt/format.h"
#include "fmt/printf.h"
#include <functional>
#include <etl_types.h>
#include <commands.h>
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
  commandQueue = xQueueCreate(maxMessagesInQueue, sizeof(LongString));

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
}

void Debug::printMessage(DebugLevel level, fmt::CStringRef format,
                         fmt::ArgList args) {
  auto formattedString = ShortString(fmt::sprintf(format, args).c_str());
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
    ShortString *commandStringAddress;
    if (xQueueReceive(commandQueue, &commandStringAddress, portMAX_DELAY)) {

      auto commandString = std::unique_ptr<ShortString>(commandStringAddress);
      bool commandRan = false;
      for (const DebugCommand &command : DebugCommands::registeredCommands) {
        if (commandString->rfind(command.name, 0) != 0) {
          continue;
        }

        if (commandString->length() > strlen(command.name)) {
          const ShortString args = commandString->substr(strlen(command.name));
          debugI("Matched Command `%s` with args `%s`", command.name, args.c_str());
          command.run(args);
        } else {
          const ShortString empty;
          debugI("Matched Command `%s`", command.name);
          command.run(empty);
        }

        commandRan = true;
        break;
      }
      if (!commandRan) {
        debugE("Unknown Command: %s", commandString->c_str());
      }
    }
  }
}

bool isInvalidChar(int c) {
  return !(c >= 0 && c < 128);
}

void stripUnicode(ShortString &str) {
  str.erase(std::remove_if(str.begin(), str.end(), isInvalidChar), str.end());
}

[[noreturn]] void Debug::messageBroadcastTask() {

  DebugMessage *queueMessage;
  if (messageQueue == nullptr) {
    debugE("Attempted to start broadcast task without queue");
  }

  for (;;) {
    if (xQueueReceive(messageQueue, &queueMessage, portMAX_DELAY)) {
      auto message = std::unique_ptr<DebugMessage>(queueMessage);

      message->content.repair(); //Required by ETL after memcpy

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
        if(info->len > ShortString::MAX_SIZE){
          debugE("Debug command longer than max length %d", ShortString::MAX_SIZE);
          break;
        }
        auto *payloadText = new ShortString(reinterpret_cast<char *>(data), info->len);
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