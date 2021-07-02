idf_component_register(SRCS "driver.cpp" "debug.cpp" "main.cpp" "motion.cpp" "motionHoming.cpp" "ota.cpp"
        INCLUDE_DIRS ""
        REQUIRES arduino esp_wifi esp_ipc esp_rom wpa_supplicant external-libraries)
