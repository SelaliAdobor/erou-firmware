option(BUILD_SHARED_LIBS OFF)
option(BUILD_TESTING OFF)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++17")


idf_component_register(SRCS
        "src/api/api.cpp"
        "src/api/apiContainer.cpp"
        "src/api/apiScheduler.cpp"
        "src/debug/wsdebug.cpp"
        "src/main.cpp"
        "src/motion/motion.cpp"
        "src/motion/motionHoming.cpp"
        "src/ota/ota.cpp"
        "src/container/containerManager.cpp"
        "src/container/container.cpp"
        "src/container/containerManagerSerialization.cpp"
        "src/dispense/dispenserManager.cpp"
        "src/dispense/dispenserManagerSerialization.cpp"
        "src/dispense/dispense.cpp"
        "src/storedSettings/storedSettings.cpp"

        INCLUDE_DIRS "src"
        "src/debug"
        "src/container"
        "src/freertos"
        "src/math"
        "src/motion"
        "src/api"
        "src/dispense"
        "src/storedSettings"
        "src/json"
        REQUIRES arduino-esp32 esp_wifi esp_ipc esp_rom wpa_supplicant external-libraries fmt-esp32)


function(install)
endfunction()

function(export)
endfunction()

add_subdirectory(lib/etl)
add_subdirectory(lib/json11)
add_subdirectory(lib/croncpp)

target_link_libraries(${COMPONENT_LIB} PUBLIC etl json11 croncpp)