#include <functional>

#include "Arduino.h"
#include "freertos/task.h"
#include "ESPAsyncWebServer.h"
#include "config_constants.h"
#include "debug.h"
#include "esp32-hal.h"
#include "esp_wifi.h"
#include "motion.h"
#include "ota.h"
#include "ota_credentials.h"
#include "pins.h"


TMC2209Stepper driver(&Serial2, 0.11F, 0b00);

ESP_FlexyStepper stepper;
Ota *ota;

Motion motion = Motion(&driver, &stepper); // NOLINT(cppcoreguidelines-slicing)
AsyncWebServer server(80);
void setupDebugCommands();

[[noreturn]] void runningBlinkTask(void *) {
  debugV("blink task starting");
  for (;;) {
    // digitalWrite(pins::onboardLed, HIGH);
    delay(config::blinkTask::blinkInterval);
    //  digitalWrite(pins::onboardLed, LOW);
    delay(config::blinkTask::blinkInterval);
  }
}



[[noreturn]]  void secondaryLoopTask(void *) {
  for (;;) {
    delay(500);
  }
}

void setupWifi() {
  Serial.begin(config::serial::baud);
  WiFiClass::mode(WIFI_STA);
  // Helps manage wifi tx power
  esp_wifi_set_max_tx_power(8);
  WiFi.begin(ssid, password);

  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    debugV("setting up wifi");
    delay(5000);
    ESP.restart();
  }
}

extern "C" {
void app_main(void) {
  initArduino();
  setup();
}
}
// IO pin assignments
const int MOTOR_STEP_PIN = 33;
const int MOTOR_DIRECTION_PIN = 25;
const int EMERGENCY_STOP_PIN = 13; //define the IO pin the emergency stop switch is connected to
const int LIMIT_SWITCH_PIN = 32;   //define the IO pin where the limit switches are connected to (switches in series in normally closed setup against ground)

// Speed settings
const int DISTANCE_TO_TRAVEL_IN_STEPS = 2000;
const int SPEED_IN_STEPS_PER_SECOND = 800;
const int ACCELERATION_IN_STEPS_PER_SECOND = 800;
const int DECELERATION_IN_STEPS_PER_SECOND = 800;
void setup() {
  pins::setup();
  setupWifi();
  debugI("setting up pins");
  // In some cases code can cause the entire board to restart,
  // This delay allows a window for new programming
  delay(config::startupDelay);
  stepper.connectToPins(pins::stepper::step, pins::stepper::direction);
  // set the speed and acceleration rates for the stepper motor
  stepper.setSpeedInStepsPerSecond(SPEED_IN_STEPS_PER_SECOND);
  stepper.setAccelerationInStepsPerSecondPerSecond(ACCELERATION_IN_STEPS_PER_SECOND);
  stepper.setDecelerationInStepsPerSecondPerSecond(DECELERATION_IN_STEPS_PER_SECOND);


  debugInstance.setup(server);
  setupDebugCommands();
  server.begin();
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/plain", "Hello, world");
  });
  delay(5000);


  debugI("setting up motion");
  motion.setup();


  debugI("starting secondary loop task");
  TaskHandle_t loopHandle;
//  xTaskCreateUniversal(secondaryLoopTask,
//                       "Loop Task",
//                       config::loopTask::stackSize,
//                       nullptr,
//                       config::loopTask::priority,
//                       &loopHandle,
//                       config::loopTask::core);

  debugI("setting up ota");
  Ota::setup();

  debugI("setup complete");
}
int coastingAcceleration = 4000;
 int instantAcceleration = 4000;
void setupDebugCommands() {
  debugInstance.registerCommand("en ", [](const std::string *args) {
    uint8_t enabled = 0;
    if (sscanf(args->c_str(), "%" SCNu8, &enabled) != 1) {
      debugI("Failed to parse command.");
      return;
    }

    digitalWrite(pins::stepper::enable, enabled);
    debugI("Updated stepper motor enabled state %d", enabled);
  });
  debugInstance.registerCommand("gtc ", [](const std::string *args) {
    int containerIndex = 0;

    if (sscanf(args->c_str(), "%d", &containerIndex) != 1) {
      debugI("Failed to parse command.");
      return;
    }

    debugI("Going to container %d for debug command", containerIndex);
    if (containerIndex >= config::physical::containerCount) {
      debugE(
          "Container index exceeded for goToContainer: %d out of %d possible",
          containerIndex, config::physical::containerCount);
      return;
    }
    motion.goToContainerAt(containerIndex);
    debugI("Went to container %d after debug command", containerIndex);
  });

}