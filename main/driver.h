//
// Created by Selali Adobor on 6/30/21.
//

#ifndef HELLO_WORLD_MAIN_STEPPER_H_
#define HELLO_WORLD_MAIN_STEPPER_H_
#include <bitset>
#include <driver/uart.h>
#include "driver.h"
#include "debug.h"
struct RegisterRead {
  uint8_t tmcRegister;
};
struct RegisterReply {
  uint8_t sync;
  std::bitset<8> address;
  std::bitset<8> tmcRegister;
  std::bitset<32> data;
  bool crcMatched;

};
enum GeneralConfiguration{
  IOIN = 0x06,
};
class Driver {

 private:
  const uint8_t tmcSyncValue = 0b10100000;
  const int replySize = 8;
  const int requestSize = 4;
  const uart_port_t uartPort = UART_NUM_2;
  uart_config_t uartConfig = {
      .baud_rate = 250000,
      .data_bits = UART_DATA_8_BITS,
      .parity = UART_PARITY_DISABLE,
      .stop_bits = UART_STOP_BITS_1,
      .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
      .rx_flow_ctrl_thresh = 0,
      .source_clk = UART_SCLK_APB
  };
  const int uartBufferSize = (1024 * 2);
  QueueHandle_t uartEventQueue;
  QueueHandle_t readDataQueue;
  bool isSetup = false;
  [[noreturn]] void rxTask();
  static void rxTaskWrapper(void *);
  uint8_t address;
  uint8_t rxPin;
  uint8_t txPin;
 public:
  Driver(uint8_t address, uint8_t rxPin, uint8_t txPin) :
      address(address), rxPin(rxPin), txPin(txPin) {}
  void setup();
  bool write(const uint8_t *data, int length) const;
  bool writeReadAccess(RegisterRead datagram) const;
  static uint8_t calcCRC(const uint8_t *datagram, uint8_t len) ;
  void readNext(RegisterReply *reply);
};

#endif //HELLO_WORLD_MAIN_STEPPER_H_
