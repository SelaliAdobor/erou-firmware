# The following lines of boilerplate have to be in your project's
# CMakeLists in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.5)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++17")

idf_build_set_property(CXX_COMPILE_OPTIONS "-std=gnu++17" "-Os" APPEND)
set_target_properties(${COMPONENT_TARGET} PROPERTIES LINK_INTERFACE_MULTIPLICITY 10)
project(erou-firmware-esp32)
add_custom_command(
        OUTPUT components/external-libraries/CMakeLists.txt
        COMMAND python generate_library_cmake.py
        DEPENDS generate_library_cmake.py)