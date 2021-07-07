set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_STANDARD_REQUIRED 17)

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_EXPORT_COMPILE_COMMANDS ENABLED)
set(CMAKE_C_FLAGS "-mlongcalls -Wno-frame-address" CACHE STRING "C Compiler Base Flags")
set(CMAKE_CXX_FLAGS "-mlongcalls -Wno-frame-address" CACHE STRING "C++ Compiler Base Flags")

idf_component_register(SRCS "debug.cpp" "main.cpp" "motion.cpp" "motionHoming.cpp" "ota.cpp"
        INCLUDE_DIRS "."
        REQUIRES arduino esp_wifi esp_ipc esp_rom wpa_supplicant external-libraries)
