# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.19

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Disable VCS-based implicit rules.
% : %,v


# Disable VCS-based implicit rules.
% : RCS/%


# Disable VCS-based implicit rules.
% : RCS/%,v


# Disable VCS-based implicit rules.
% : SCCS/s.%


# Disable VCS-based implicit rules.
% : s.%


.SUFFIXES: .hpux_make_needs_suffix_list


# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake

# The command to remove a file.
RM = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/oak/Development/erou-firmware-esp32

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/oak/Development/erou-firmware-esp32/cmake-build-debug

# Utility rule file for __ldgen_output_esp32.project.ld.

# Include the progress variables for this target.
include esp-idf/esp32/CMakeFiles/__ldgen_output_esp32.project.ld.dir/progress.make

esp-idf/esp32/CMakeFiles/__ldgen_output_esp32.project.ld: esp-idf/esp32/ld/esp32.project.ld


esp-idf/esp32/ld/esp32.project.ld: /Users/oak/esp/esp-idf/components/esp32/ld/esp32.project.ld.in
esp-idf/esp32/ld/esp32.project.ld: /Users/oak/esp/esp-idf/components/spi_flash/linker.lf
esp-idf/esp32/ld/esp32.project.ld: /Users/oak/esp/esp-idf/components/esp_gdbstub/linker.lf
esp-idf/esp32/ld/esp32.project.ld: /Users/oak/esp/esp-idf/components/espcoredump/linker.lf
esp-idf/esp32/ld/esp32.project.ld: /Users/oak/esp/esp-idf/components/esp_phy/linker.lf
esp-idf/esp32/ld/esp32.project.ld: /Users/oak/esp/esp-idf/components/esp_system/linker.lf
esp-idf/esp32/ld/esp32.project.ld: /Users/oak/esp/esp-idf/components/esp_system/app.lf
esp-idf/esp32/ld/esp32.project.ld: /Users/oak/esp/esp-idf/components/hal/linker.lf
esp-idf/esp32/ld/esp32.project.ld: /Users/oak/esp/esp-idf/components/esp_event/linker.lf
esp-idf/esp32/ld/esp32.project.ld: /Users/oak/esp/esp-idf/components/esp_wifi/linker.lf
esp-idf/esp32/ld/esp32.project.ld: /Users/oak/esp/esp-idf/components/lwip/linker.lf
esp-idf/esp32/ld/esp32.project.ld: /Users/oak/esp/esp-idf/components/log/linker.lf
esp-idf/esp32/ld/esp32.project.ld: /Users/oak/esp/esp-idf/components/heap/linker.lf
esp-idf/esp32/ld/esp32.project.ld: /Users/oak/esp/esp-idf/components/soc/linker.lf
esp-idf/esp32/ld/esp32.project.ld: /Users/oak/esp/esp-idf/components/esp_hw_support/linker.lf
esp-idf/esp32/ld/esp32.project.ld: /Users/oak/esp/esp-idf/components/esp_pm/linker.lf
esp-idf/esp32/ld/esp32.project.ld: /Users/oak/esp/esp-idf/components/esp_ringbuf/linker.lf
esp-idf/esp32/ld/esp32.project.ld: /Users/oak/esp/esp-idf/components/driver/linker.lf
esp-idf/esp32/ld/esp32.project.ld: /Users/oak/esp/esp-idf/components/xtensa/linker.lf
esp-idf/esp32/ld/esp32.project.ld: /Users/oak/esp/esp-idf/components/esp_common/common.lf
esp-idf/esp32/ld/esp32.project.ld: /Users/oak/esp/esp-idf/components/esp_common/soc.lf
esp-idf/esp32/ld/esp32.project.ld: /Users/oak/esp/esp-idf/components/freertos/linker.lf
esp-idf/esp32/ld/esp32.project.ld: /Users/oak/esp/esp-idf/components/newlib/newlib.lf
esp-idf/esp32/ld/esp32.project.ld: /Users/oak/esp/esp-idf/components/newlib/system_libs.lf
esp-idf/esp32/ld/esp32.project.ld: /Users/oak/esp/esp-idf/components/app_trace/linker.lf
esp-idf/esp32/ld/esp32.project.ld: /Users/oak/esp/esp-idf/components/bt/linker.lf
esp-idf/esp32/ld/esp32.project.ld: esp-idf/mbedtls/libmbedtls.a
esp-idf/esp32/ld/esp32.project.ld: esp-idf/efuse/libefuse.a
esp-idf/esp32/ld/esp32.project.ld: esp-idf/app_update/libapp_update.a
esp-idf/esp32/ld/esp32.project.ld: esp-idf/bootloader_support/libbootloader_support.a
esp-idf/esp32/ld/esp32.project.ld: esp-idf/esp_ipc/libesp_ipc.a
esp-idf/esp32/ld/esp32.project.ld: esp-idf/spi_flash/libspi_flash.a
esp-idf/esp32/ld/esp32.project.ld: esp-idf/nvs_flash/libnvs_flash.a
esp-idf/esp32/ld/esp32.project.ld: esp-idf/pthread/libpthread.a
esp-idf/esp32/ld/esp32.project.ld: esp-idf/esp_gdbstub/libesp_gdbstub.a
esp-idf/esp32/ld/esp32.project.ld: esp-idf/espcoredump/libespcoredump.a
esp-idf/esp32/ld/esp32.project.ld: esp-idf/esp_phy/libesp_phy.a
esp-idf/esp32/ld/esp32.project.ld: esp-idf/esp_system/libesp_system.a
esp-idf/esp32/ld/esp32.project.ld: esp-idf/esp_rom/libesp_rom.a
esp-idf/esp32/ld/esp32.project.ld: esp-idf/hal/libhal.a
esp-idf/esp32/ld/esp32.project.ld: esp-idf/vfs/libvfs.a
esp-idf/esp32/ld/esp32.project.ld: esp-idf/esp_eth/libesp_eth.a
esp-idf/esp32/ld/esp32.project.ld: esp-idf/tcpip_adapter/libtcpip_adapter.a
esp-idf/esp32/ld/esp32.project.ld: esp-idf/esp_netif/libesp_netif.a
esp-idf/esp32/ld/esp32.project.ld: esp-idf/esp_event/libesp_event.a
esp-idf/esp32/ld/esp32.project.ld: esp-idf/wpa_supplicant/libwpa_supplicant.a
esp-idf/esp32/ld/esp32.project.ld: esp-idf/esp_wifi/libesp_wifi.a
esp-idf/esp32/ld/esp32.project.ld: esp-idf/lwip/liblwip.a
esp-idf/esp32/ld/esp32.project.ld: esp-idf/log/liblog.a
esp-idf/esp32/ld/esp32.project.ld: esp-idf/heap/libheap.a
esp-idf/esp32/ld/esp32.project.ld: esp-idf/soc/libsoc.a
esp-idf/esp32/ld/esp32.project.ld: esp-idf/esp_hw_support/libesp_hw_support.a
esp-idf/esp32/ld/esp32.project.ld: esp-idf/esp_pm/libesp_pm.a
esp-idf/esp32/ld/esp32.project.ld: esp-idf/esp_ringbuf/libesp_ringbuf.a
esp-idf/esp32/ld/esp32.project.ld: esp-idf/driver/libdriver.a
esp-idf/esp32/ld/esp32.project.ld: esp-idf/xtensa/libxtensa.a
esp-idf/esp32/ld/esp32.project.ld: esp-idf/perfmon/libperfmon.a
esp-idf/esp32/ld/esp32.project.ld: esp-idf/esp32/libesp32.a
esp-idf/esp32/ld/esp32.project.ld: esp-idf/esp_common/libesp_common.a
esp-idf/esp32/ld/esp32.project.ld: esp-idf/esp_timer/libesp_timer.a
esp-idf/esp32/ld/esp32.project.ld: esp-idf/freertos/libfreertos.a
esp-idf/esp32/ld/esp32.project.ld: esp-idf/newlib/libnewlib.a
esp-idf/esp32/ld/esp32.project.ld: esp-idf/cxx/libcxx.a
esp-idf/esp32/ld/esp32.project.ld: esp-idf/app_trace/libapp_trace.a
esp-idf/esp32/ld/esp32.project.ld: esp-idf/asio/libasio.a
esp-idf/esp32/ld/esp32.project.ld: esp-idf/cbor/libcbor.a
esp-idf/esp32/ld/esp32.project.ld: esp-idf/unity/libunity.a
esp-idf/esp32/ld/esp32.project.ld: esp-idf/cmock/libcmock.a
esp-idf/esp32/ld/esp32.project.ld: esp-idf/coap/libcoap.a
esp-idf/esp32/ld/esp32.project.ld: esp-idf/console/libconsole.a
esp-idf/esp32/ld/esp32.project.ld: esp-idf/nghttp/libnghttp.a
esp-idf/esp32/ld/esp32.project.ld: esp-idf/esp-tls/libesp-tls.a
esp-idf/esp32/ld/esp32.project.ld: esp-idf/esp_adc_cal/libesp_adc_cal.a
esp-idf/esp32/ld/esp32.project.ld: esp-idf/esp_hid/libesp_hid.a
esp-idf/esp32/ld/esp32.project.ld: esp-idf/tcp_transport/libtcp_transport.a
esp-idf/esp32/ld/esp32.project.ld: esp-idf/esp_http_client/libesp_http_client.a
esp-idf/esp32/ld/esp32.project.ld: esp-idf/esp_http_server/libesp_http_server.a
esp-idf/esp32/ld/esp32.project.ld: esp-idf/esp_https_ota/libesp_https_ota.a
esp-idf/esp32/ld/esp32.project.ld: esp-idf/esp_https_server/libesp_https_server.a
esp-idf/esp32/ld/esp32.project.ld: esp-idf/esp_lcd/libesp_lcd.a
esp-idf/esp32/ld/esp32.project.ld: esp-idf/protobuf-c/libprotobuf-c.a
esp-idf/esp32/ld/esp32.project.ld: esp-idf/protocomm/libprotocomm.a
esp-idf/esp32/ld/esp32.project.ld: esp-idf/mdns/libmdns.a
esp-idf/esp32/ld/esp32.project.ld: esp-idf/esp_local_ctrl/libesp_local_ctrl.a
esp-idf/esp32/ld/esp32.project.ld: esp-idf/sdmmc/libsdmmc.a
esp-idf/esp32/ld/esp32.project.ld: esp-idf/esp_serial_slave_link/libesp_serial_slave_link.a
esp-idf/esp32/ld/esp32.project.ld: esp-idf/esp_websocket_client/libesp_websocket_client.a
esp-idf/esp32/ld/esp32.project.ld: esp-idf/expat/libexpat.a
esp-idf/esp32/ld/esp32.project.ld: esp-idf/wear_levelling/libwear_levelling.a
esp-idf/esp32/ld/esp32.project.ld: esp-idf/fatfs/libfatfs.a
esp-idf/esp32/ld/esp32.project.ld: esp-idf/freemodbus/libfreemodbus.a
esp-idf/esp32/ld/esp32.project.ld: esp-idf/jsmn/libjsmn.a
esp-idf/esp32/ld/esp32.project.ld: esp-idf/json/libjson.a
esp-idf/esp32/ld/esp32.project.ld: esp-idf/libsodium/liblibsodium.a
esp-idf/esp32/ld/esp32.project.ld: esp-idf/mqtt/libmqtt.a
esp-idf/esp32/ld/esp32.project.ld: esp-idf/openssl/libopenssl.a
esp-idf/esp32/ld/esp32.project.ld: esp-idf/spiffs/libspiffs.a
esp-idf/esp32/ld/esp32.project.ld: esp-idf/ulp/libulp.a
esp-idf/esp32/ld/esp32.project.ld: esp-idf/wifi_provisioning/libwifi_provisioning.a
esp-idf/esp32/ld/esp32.project.ld: esp-idf/arduino/libarduino.a
esp-idf/esp32/ld/esp32.project.ld: esp-idf/external-libraries/libexternal-libraries.a
esp-idf/esp32/ld/esp32.project.ld: esp-idf/main/libmain.a
esp-idf/esp32/ld/esp32.project.ld: /Users/oak/Development/erou-firmware-esp32/sdkconfig
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/Users/oak/Development/erou-firmware-esp32/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Generating ld/esp32.project.ld"
	cd /Users/oak/Development/erou-firmware-esp32/cmake-build-debug/esp-idf/esp32 && python /Users/oak/esp/esp-idf/tools/ldgen/ldgen.py --config /Users/oak/Development/erou-firmware-esp32/sdkconfig --fragments /Users/oak/esp/esp-idf/components/spi_flash/linker.lf	/Users/oak/esp/esp-idf/components/esp_gdbstub/linker.lf	/Users/oak/esp/esp-idf/components/espcoredump/linker.lf	/Users/oak/esp/esp-idf/components/esp_phy/linker.lf	/Users/oak/esp/esp-idf/components/esp_system/linker.lf	/Users/oak/esp/esp-idf/components/esp_system/app.lf	/Users/oak/esp/esp-idf/components/hal/linker.lf	/Users/oak/esp/esp-idf/components/esp_event/linker.lf	/Users/oak/esp/esp-idf/components/esp_wifi/linker.lf	/Users/oak/esp/esp-idf/components/lwip/linker.lf	/Users/oak/esp/esp-idf/components/log/linker.lf	/Users/oak/esp/esp-idf/components/heap/linker.lf	/Users/oak/esp/esp-idf/components/soc/linker.lf	/Users/oak/esp/esp-idf/components/esp_hw_support/linker.lf	/Users/oak/esp/esp-idf/components/esp_pm/linker.lf	/Users/oak/esp/esp-idf/components/esp_ringbuf/linker.lf	/Users/oak/esp/esp-idf/components/driver/linker.lf	/Users/oak/esp/esp-idf/components/xtensa/linker.lf	/Users/oak/esp/esp-idf/components/esp_common/common.lf	/Users/oak/esp/esp-idf/components/esp_common/soc.lf	/Users/oak/esp/esp-idf/components/freertos/linker.lf	/Users/oak/esp/esp-idf/components/newlib/newlib.lf	/Users/oak/esp/esp-idf/components/newlib/system_libs.lf	/Users/oak/esp/esp-idf/components/app_trace/linker.lf	/Users/oak/esp/esp-idf/components/bt/linker.lf --input /Users/oak/esp/esp-idf/components/esp32/ld/esp32.project.ld.in --output /Users/oak/Development/erou-firmware-esp32/cmake-build-debug/esp-idf/esp32/ld/esp32.project.ld --kconfig /Users/oak/esp/esp-idf/Kconfig --env-file /Users/oak/Development/erou-firmware-esp32/cmake-build-debug/config.env --libraries-file /Users/oak/Development/erou-firmware-esp32/cmake-build-debug/ldgen_libraries --objdump /Users/oak/.espressif/tools/xtensa-esp32-elf/esp-2020r3-8.4.0/xtensa-esp32-elf/bin/xtensa-esp32-elf-objdump

__ldgen_output_esp32.project.ld: esp-idf/esp32/CMakeFiles/__ldgen_output_esp32.project.ld
__ldgen_output_esp32.project.ld: esp-idf/esp32/ld/esp32.project.ld
__ldgen_output_esp32.project.ld: esp-idf/esp32/CMakeFiles/__ldgen_output_esp32.project.ld.dir/build.make

.PHONY : __ldgen_output_esp32.project.ld

# Rule to build all files generated by this target.
esp-idf/esp32/CMakeFiles/__ldgen_output_esp32.project.ld.dir/build: __ldgen_output_esp32.project.ld

.PHONY : esp-idf/esp32/CMakeFiles/__ldgen_output_esp32.project.ld.dir/build

esp-idf/esp32/CMakeFiles/__ldgen_output_esp32.project.ld.dir/clean:
	cd /Users/oak/Development/erou-firmware-esp32/cmake-build-debug/esp-idf/esp32 && $(CMAKE_COMMAND) -P CMakeFiles/__ldgen_output_esp32.project.ld.dir/cmake_clean.cmake
.PHONY : esp-idf/esp32/CMakeFiles/__ldgen_output_esp32.project.ld.dir/clean

esp-idf/esp32/CMakeFiles/__ldgen_output_esp32.project.ld.dir/depend:
	cd /Users/oak/Development/erou-firmware-esp32/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/oak/Development/erou-firmware-esp32 /Users/oak/esp/esp-idf/components/esp32 /Users/oak/Development/erou-firmware-esp32/cmake-build-debug /Users/oak/Development/erou-firmware-esp32/cmake-build-debug/esp-idf/esp32 /Users/oak/Development/erou-firmware-esp32/cmake-build-debug/esp-idf/esp32/CMakeFiles/__ldgen_output_esp32.project.ld.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : esp-idf/esp32/CMakeFiles/__ldgen_output_esp32.project.ld.dir/depend

