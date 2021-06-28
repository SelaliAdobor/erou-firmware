# The following lines of boilerplate have to be in your project's
# CMakeLists in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.5)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)

set_target_properties(${COMPONENT_TARGET} PROPERTIES LINK_INTERFACE_MULTIPLICITY 10)
project(erou-firmware-esp32)
