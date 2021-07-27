#include "wsdebug.h"
#include <deque>
#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>
#include <new>
#include <functional>
#include <etl_types.h>
#include <commands.h>
#include <api.h>
#include "free_rtos_util.h"

Debug debugInstance = Debug();

//ESP-IDF callback for debug statements
int esp_apptrace_vprintf(const char *fmt, va_list ap);

void Debug::setup() {
  esp_log_set_vprintf(esp_apptrace_vprintf);
  em::registerWsRoute("/debug", em::method::Get, [this](const em::WsMessage &message) {
    if (message.text.size() > ShortString::MAX_SIZE) {
      debugE(logtags::debug,"Debug command longer than max length %d", ShortString::MAX_SIZE);
      return;
    }
    char *data = strdup(message.text.data());
    auto *payloadText = new LongString(data);
    xQueueSend(commandQueue, &payloadText, pdMS_TO_TICKS(500));
  });
}

Debug::Debug() {

  messageQueue = xQueueCreate(
      maxMessagesInQueue,
      sizeof(DebugMessage *) // NOLINT(bugprone-sizeof-expression) FreeRTOS makes you do weird things
  );

  commandQueue = xQueueCreate(maxMessagesInQueue, sizeof(LongString *));

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

[[noreturn]] void Debug::commandRunnerTask() {
  if (commandQueue == nullptr) {
    debugE(logtags::debug, "Attempted to start command task without queue");
  }
  for (;;) {
    LongString *commandStringAddress;
    if (!xQueueReceive(commandQueue, &commandStringAddress, portMAX_DELAY)) {
      continue; //MAX_DELAY means this won't cause WDT issues
    }

    auto commandString = std::unique_ptr<LongString>(commandStringAddress);
    bool commandRan = false;
    for (const DebugCommand &command : DebugCommands::registeredCommands) {
      if (commandString->find(command.name.data()) != 0) {
        debugI(logtags::debug, "Failed to match command `%s`: `%d`", commandString->c_str(), commandString->find(command.name.data()));
        continue;
      }

      if (commandString->length() > command.name.length()) {
        const LongString args = commandString->substr(command.name.length());
        debugI(logtags::debug, "Matched Command `%s` with args `%s`", command.name.data(), args.c_str());
        command.run(args);
      } else {
        const LongString empty;
        debugI(logtags::debug, "Matched Command `%s`", command.name.data());
        command.run(empty);
      }

      commandRan = true;
      break;
    }
    if (!commandRan) {
      debugE(logtags::debug, "Unknown Command: %s", commandString->c_str());
    }
  }
}

bool isInvalidChar(int c) {
  return !(c >= 0 && c < 128);
}

void stripUnicode(LongString &str) {
  str.erase(std::remove_if(str.begin(), str.end(), isInvalidChar), str.end());
}

[[noreturn]] void Debug::messageBroadcastTask() {
  if (messageQueue == nullptr) {
    debugE(logtags::debug,"Attempted to start broadcast task without queue");
  }
  DebugMessage *queueMessage = nullptr;
  for (;;) {
    if (xQueueReceive(messageQueue, &queueMessage, portMAX_DELAY)) {

      auto message = std::unique_ptr<DebugMessage>(queueMessage);
      auto messageContent = std::unique_ptr<char>(queueMessage->content);

      if (message->level >= loggingLevel) {
        LongString unicodeString = LongString(messageContent.get());
        stripUnicode(unicodeString);
        em::sendAllWs("/debug", std::string_view(unicodeString.c_str()));
      }
      delay(500);
    }
  }
}

bool startsWith(const char *str, const char *pre) {
  return strncmp(pre, str, strlen(pre)) == 0;
}
#ifdef ESP_ASYNC
void Debug::handleWsEvent(AsyncWebSocketRequest * request,
                          AsyncWebSocketClient *client,
                          int type,
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
        if (info->len > ShortString::MAX_SIZE) {
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
#endif

int esp_apptrace_vprintf(const char *fmt, va_list ap) {
  char espString[50];
  int written = vsnprintf(espString, 50, fmt, ap);
  debugESP("ESP Log: %s", espString);
  return written;
}