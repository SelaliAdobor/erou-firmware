option(BUILD_SHARED_LIBS OFF)
option(BUILD_TESTING OFF)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++17")


idf_component_register(SRCS
        "src/api/api.cpp"
        "src/api/apiContainer.cpp"
        "src/api/apiScheduler.cpp"
        "src/api/easymongoose/response.cpp"
        "src/api/easymongoose/easymongoose.cpp"
        "src/api/easymongoose/easymongooseHandlers.cpp"
        "src/debug/wsdebug.cpp"
        "src/debug/commands/goToContainer.cpp"
        "src/debug/commands/setDebugLevel.cpp"
        "src/debug/commands/setStepperEnable.cpp"
        "src/debug/commands/uptime.cpp"
        "src/main.cpp"
        "src/motion/motion.cpp"
        "src/motion/motionHoming.cpp"
        "src/ota/ota.cpp"
        "src/container/containerManager.cpp"
        "src/container/containerManagerSerialization.cpp"
        "src/dispense/dispenserManager.cpp"
        "src/dispense/dispenserManagerSerialization.cpp"
        "src/dispense/dispense.cpp"
        "src/storedSettings/storedSettings.cpp"

        INCLUDE_DIRS
        "src"
        "src/debug"
        "src/debug/commands"
        "src/container"
        "src/freertos"
        "src/math"
        "src/motion"
        "src/api"
        "src/api/easymongoose"
        "src/dispense"
        "src/storedSettings"
        "src/json"
        "src/etl"


        REQUIRES arduino-esp32 ArduinoJson esp_wifi esp_ipc esp_rom wpa_supplicant external-libraries fmt-esp32)


function(install)
endfunction()

function(export)
endfunction()

add_subdirectory(lib/etl)
add_subdirectory(lib/json11)
add_subdirectory(lib/croncpp)

target_link_libraries(${COMPONENT_LIB} PUBLIC etl json11 croncpp)