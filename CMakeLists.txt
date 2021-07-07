# The following lines of boilerplate have to be in your project's
# CMakeLists in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.5)
set(CMAKE_SIZEOF_VOID_P 4)
include($ENV{IDF_PATH}/tools/cmake/project.cmake)
set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_STANDARD_REQUIRED 17)

set(CMAKE_SYSTEM_NAME Generic)

set(CMAKE_C_COMPILER ~/.espressif/tools/xtensa-esp32-elf/esp-2020r3-8.4.0/xtensa-esp32-elf/bin/xtensa-esp32-elf-gcc)
set(CMAKE_CXX_COMPILER ~/.espressif/tools/xtensa-esp32-elf/esp-2020r3-8.4.0/xtensa-esp32-elf/bin/xtensa-esp32-elf-g++)
set(CMAKE_ASM_COMPILER ~/.espressif/tools/xtensa-esp32-elf/esp-2020r3-8.4.0/xtensa-esp32-elf/bin/xtensa-esp32-elf-gcc)
set(CMAKE_EXPORT_COMPILE_COMMANDS true)
set_target_properties(${COMPONENT_TARGET} PROPERTIES LINK_INTERFACE_MULTIPLICITY 10)
project(erou-firmware-esp32)
