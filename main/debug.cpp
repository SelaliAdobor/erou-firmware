#include "debug.h"

#include <freertos/FreeRTOS.h>

#include "fmt/format.h"
#include "fmt/printf.h"

Debug DebugInstance = Debug();


Debug::Debug() {
  messageAcessSemaphore = xSemaphoreCreateMutex();

  xTaskCreate(messageBroadcastTaskWrapper,  // Function that should be called
              "Websocket Debug Task",       // Name of the task (for debugging)
              config::debug::wsTaskStack,   // Stack size (bytes)
              this,                         // Parameter to pass
              config::debug::wsTaskPriority,  // Task priority
              &messageLoopHandle              // Task handle
  );

  registerCommand("level ", [this](std::stringstream args) {
    int newLevel = 0;
    args >> newLevel;
    loggingLevel = DebugLevel(newLevel);
    debugI("Setting log level to %d", loggingLevel);
  });
}

void Debug::printMessage(DebugLevel level, fmt::CStringRef format,
                         fmt::ArgList args) {
  std::string formattedString = fmt::sprintf(format, args);
  DebugMessage message;
  message.content = formattedString;
  message.level = level;

  messageQueue.enqueue(message);
  xTaskNotifyGive(messageLoopHandle);
}

void Debug::messageBroadcastTaskWrapper(void *_this) {
  static_cast<Debug *>(_this)->messageBroadcastTask();
}

void Debug::messageBroadcastTask() {
  for (;;) {
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

    DebugMessage message;
    while (messageQueue.try_dequeue(message)) {
      if (message.level >= loggingLevel) {
     //   webSocket.broadcastTXT(message.content.c_str());
        Serial.println(message.content.c_str());
      }
    }
  }
}
bool startsWith(const char *str, const char *pre) {
  return strncmp(pre, str, strlen(pre)) == 0;
}
void Debug::registerCommand(std::string name,
                            std::function<void(std::stringstream)> run) {
  DebugCommand newCommand = {name, run};
  commands.push_back(DebugCommand(newCommand));
}
//
//void Debug::handleWsEvent(uint8_t num, WStype_t type, uint8_t *payload,
//                          size_t length) {
//  std::string payloadText;
//  switch (type) {
//    case WStype_DISCONNECTED:
//      Serial.printf("Debug Websocket Client Disconnected: [%u]\n", num);
//      break;
//    case WStype_CONNECTED: {
//      IPAddress ip = webSocket.remoteIP(num);
//      Serial.printf(
//          "Debug Websocket Client Connected: [%u] %d.%d.%d.%d url: %s\n", num,
//          ip[0], ip[1], ip[2], ip[3], payload);
//
//      webSocket.sendTXT(num, "Erou Debug Session:");
//    } break;
//    case WStype_TEXT:
//      payloadText = std::string((char *)payload);
//
//      Serial.printf("Debug Websocket Client Sent Text: [%u] %s\n", num,
//                    payload);
//      for (DebugCommand command : commands) {
//        if (payloadText.rfind(command.name, 0) == 0) {
//          command.run(
//              std::stringstream(payloadText.substr(command.name.length())));
//          return;
//        }
//      }
//      webSocket.sendTXT(num,
//                        fmt::sprintf("Unknown Command: %s", payload).c_str());
//
//      break;
//    case WStype_BIN:
//      Serial.printf(
//          "Debug Websocket Client Sent Binary: [%u] binary length: %u\n", num,
//          length);
//      break;
//    default:
//      break;
//  }
//}
//
//void Debug::setup() {
//  webSocket.begin();
//  webSocket.onEvent(
//      [this](uint8_t num, WStype_t type, uint8_t *payload, size_t length) {
//        handleWsEvent(num, type, payload, length);
//      });
//}
//void Debug::loop() { webSocket.loop(); }