# Erou Firmware 

(working title)

Erou is a secure IoT enabled pill dispenser, and this is the firmware for it.

Written in C++ , requires FreeRTOS and builds with ESP-IDF. 

Firmware attempts to be non-opinionated towards what is being dispensed, 
which allows it to be used for non-medical purposes. 

Also avoids dependency on external backend services, device needs to be able to operate independantly of any backend service. 

Matching app should be able to provide configuration via local network.

## How to build

Use `idf.py build`

Requires C++17, included with ESP-IDF 4.x and up

## Project Structure

- `main/src` contains main project code. Some important directories:
  - `api`: contains HTTP server callbacks
  - `debug`: a websocket based debug library (TODO: Breakout into standalone library)
  - `storedSettings`: A generalized SPIFFS backed k-v store 
  - `motion`: Controls for hardware kinematics
  - `config_constants.h / pins.h`: Constant values that define device operation
- `main/lib` contains pure CMake modules. Each one must be added to `main/CmakeLists.txt`
- `components` contains ESP-IDF compatible modules. 



For more information on structure and contents of ESP-IDF projects, please refer to Section [Build System](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-guides/build-system.html) of the ESP-IDF Programming Guide.

## TODO

- [ ] Audio 
- [ ] LED indicators
- [ ] Touch sensor 

## Code Guidelines

- Prefer [ETL](https://www.etlcpp.com/) containers and strings
- Avoid using Arduino APIs, Arduino is mainly included for library dependencies 
- clang-tidy configuration included in home directory


