//
// Created by Selali Adobor on 6/30/21.
//

#include <hal/uart_types.h>
#include <driver/uart.h>
#include "driver.h"
#include "debug.h"
#include <bitset>
#include "pins.h"
void Driver::setup() {
  delay(100);
  if (int err = uart_param_config(uartPort, &uartConfig)) {
    debugE("Invalid UART config %d", err);
    return;
  }

  if (int err = uart_set_pin(uartPort, rxPin, txPin, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE)) {
    debugE("Invalid UART pins %d rx: %d tx: %d", err, rxPin, txPin);
    return;
  }

  if (int err = uart_driver_install(uartPort, uartBufferSize, uartBufferSize, 10, &uartEventQueue, 0)) {
    debugE("Failed to install uart driver %d", err);
    return;
  }
  readDataQueue = xQueueCreate(100, sizeof(RegisterReply));
  if (readDataQueue == nullptr) {
    debugE("Failed to create read queue");
  }
  xTaskCreate(rxTaskWrapper,
              "RX Task",
              config::blinkTask::stackSize,
              this,
              config::blinkTask::priority,
              nullptr
  );
  isSetup = true;

  debugI("write TMC");
  RegisterRead ioinTest{GeneralConfiguration::IOIN};
  writeReadAccess(ioinTest);
  debugI("wrote TMC");
  delay(4);

  debugI("read TMC");
  RegisterReply request;
  readNext(&request);
  debugI("Got TMC reply: %s", request.data.to_string());

}
[[noreturn]] void Driver::rxTask() {
  uart_event_t event;
  auto *data = static_cast<uint8_t *>(malloc(replySize));
  int bytesRead = 0;
  RegisterReply datagram;
  for (;;) {
    if (xQueueReceive(uartEventQueue, &event, portMAX_DELAY)) {

      debugI("Event type: %d Size: %d", event.type, event.size);
      switch (event.type) {
        case UART_DATA: {
            bytesRead += uart_read_bytes(uartPort, data + bytesRead, replySize - bytesRead, 1000);
            if (bytesRead < replySize) {
              debugE("Partial read %d", bytesRead);
            }else{
              debugE("Full read %d", bytesRead);

              datagram = {data[7],
                          data[6],
                          data[5],
                          uint32_t(
                              (data[4]) << 24 |
                                  (data[3]) << 16 |
                                  (data[2]) << 8 |
                                  (data[1])),
                          calcCRC(data, replySize) == data[0]
              };
              xQueueSend(readDataQueue, &datagram, 100);
              bytesRead = 0;
            }



          break;
        }
        case UART_FIFO_OVF: {
          debugI("hw fifo overflow");
          // If fifo overflow happened, you should consider adding flow control for your application.
          // The ISR has already reset the rx FIFO,
          // As an example, we directly flush the rx buffer here in order to read more data.
          uart_flush_input(uartPort);
        }
          break;
          //Event of UART ring buffer full
        case UART_BUFFER_FULL: {
          debugI("ring buffer full");
          // If buffer full happened, you should consider encreasing your buffer size
          // As an example, we directly flush the rx buffer here in order to read more data.
          uart_flush_input(uartPort);
          break;
        }
          //Event of UART RX break detected
        case UART_BREAK:debugI("uart rx break");
          break;
          //Event of UART parity check error
        case UART_PARITY_ERR:debugI("uart parity error");
          break;
          //Event of UART frame error
        case UART_FRAME_ERR:debugI("uart frame error");
          break;
          //UART_PATTERN_DET
          //Others
        default:debugI("uart event type: %d", event.type);
          break;
      }
    }
  }
}

bool Driver::write(const uint8_t *data, int length) const {
  if (!isSetup) {
    debugE("Attempted to write data before setup");
    return false;
  }
  auto *dataCopy = static_cast<uint8_t *>(malloc(length));
  memcpy(dataCopy, data, length);
  if (uart_write_bytes(uartPort, dataCopy, length) < length) {
    debugE("Failed to write data to port: %d", length);
    return false;
  }
  if (int err = uart_wait_tx_done(uartPort, pdMS_TO_TICKS(10000))) {
    debugE("Timed out waiting for transmission: %d", err);
    free(dataCopy);
    return false;
  }
  free(dataCopy);
  return true;
}

void Driver::readNext(RegisterReply *reply) {
  xQueueReceive(readDataQueue, reply, portMAX_DELAY);
  debugI("Read TMC Datagram, Address: %s Register: %s, Crc Matched: %d",
         reply->address.to_string(),
         reply->tmcRegister.to_string(),
         reply->crcMatched);
}
void Driver::rxTaskWrapper(void *_this) {
  static_cast<Driver *>(_this)->rxTask();
}
bool Driver::writeReadAccess(RegisterRead datagram) const {
  uint8_t serial[] = {
      tmcSyncValue,
      address,
      datagram.tmcRegister,
      0x00
  };
  serial[3] = calcCRC(serial, requestSize);
  return write(serial, requestSize);
}
uint8_t Driver::calcCRC(const uint8_t datagram[], uint8_t len) {
  uint8_t crc = 0;
  for (uint8_t i = 0; i < len; i++) {
    uint8_t currentByte = datagram[i];
    for (uint8_t j = 0; j < 8; j++) {
      if ((crc >> 7) ^ (currentByte & 0x01)) {
        crc = (crc << 1) ^ 0x07;
      } else {
        crc = (crc << 1);
      }
      crc &= 0xff;
      currentByte = currentByte >> 1;
    }
  }
  return crc;
}