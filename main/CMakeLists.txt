set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_STANDARD_REQUIRED 17)
set (CMAKE_CXX_EXTENSIONS OFF)

idf_component_register(SRCS "debug.cpp" "main.cpp" "motion.cpp" "motionHoming.cpp" "ota.cpp"
        INCLUDE_DIRS ""
        REQUIRES arduino esp_wifi esp_ipc esp_rom wpa_supplicant external-libraries)
