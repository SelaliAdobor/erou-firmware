# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.19

# compile ASM with /Users/oak/.espressif/tools/xtensa-esp32-elf/esp-2020r3-8.4.0/xtensa-esp32-elf/bin/xtensa-esp32-elf-gcc
# compile C with /Users/oak/.espressif/tools/xtensa-esp32-elf/esp-2020r3-8.4.0/xtensa-esp32-elf/bin/xtensa-esp32-elf-gcc
ASM_DEFINES = -DCONFIGURED -DHAVE_WEAK_SYMBOLS -DMBEDTLS_CONFIG_FILE=\"mbedtls/esp_config.h\" -DNATIVE_LITTLE_ENDIAN -D__STDC_CONSTANT_MACROS -D__STDC_LIMIT_MACROS

ASM_INCLUDES = -I/Users/oak/Development/erou-firmware-esp32/cmake-build-debug/config -I/Users/oak/esp/esp-idf/components/libsodium/libsodium/src/libsodium/include -I/Users/oak/esp/esp-idf/components/libsodium/port_include -I/Users/oak/esp/esp-idf/components/libsodium/libsodium/src/libsodium/include/sodium -I/Users/oak/esp/esp-idf/components/libsodium/port_include/sodium -I/Users/oak/esp/esp-idf/components/libsodium/port -I/Users/oak/esp/esp-idf/components/newlib/platform_include -I/Users/oak/esp/esp-idf/components/freertos/include -I/Users/oak/esp/esp-idf/components/freertos/port/xtensa/include -I/Users/oak/esp/esp-idf/components/esp_hw_support/include -I/Users/oak/esp/esp-idf/components/esp_hw_support/include/soc -I/Users/oak/esp/esp-idf/components/esp_hw_support/port/esp32/. -I/Users/oak/esp/esp-idf/components/heap/include -I/Users/oak/esp/esp-idf/components/log/include -I/Users/oak/esp/esp-idf/components/lwip/include/apps -I/Users/oak/esp/esp-idf/components/lwip/include/apps/sntp -I/Users/oak/esp/esp-idf/components/lwip/lwip/src/include -I/Users/oak/esp/esp-idf/components/lwip/port/esp32/include -I/Users/oak/esp/esp-idf/components/lwip/port/esp32/include/arch -I/Users/oak/esp/esp-idf/components/soc/include -I/Users/oak/esp/esp-idf/components/soc/esp32/. -I/Users/oak/esp/esp-idf/components/soc/esp32/include -I/Users/oak/esp/esp-idf/components/hal/esp32/include -I/Users/oak/esp/esp-idf/components/hal/include -I/Users/oak/esp/esp-idf/components/esp_rom/include -I/Users/oak/esp/esp-idf/components/esp_rom/esp32 -I/Users/oak/esp/esp-idf/components/esp_common/include -I/Users/oak/esp/esp-idf/components/esp_system/include -I/Users/oak/esp/esp-idf/components/esp_system/port/soc -I/Users/oak/esp/esp-idf/components/esp_system/port/public_compat -I/Users/oak/esp/esp-idf/components/esp32/include -I/Users/oak/esp/esp-idf/components/driver/include -I/Users/oak/esp/esp-idf/components/driver/esp32/include -I/Users/oak/esp/esp-idf/components/esp_pm/include -I/Users/oak/esp/esp-idf/components/esp_ringbuf/include -I/Users/oak/esp/esp-idf/components/efuse/include -I/Users/oak/esp/esp-idf/components/efuse/esp32/include -I/Users/oak/esp/esp-idf/components/xtensa/include -I/Users/oak/esp/esp-idf/components/xtensa/esp32/include -I/Users/oak/esp/esp-idf/components/vfs/include -I/Users/oak/esp/esp-idf/components/esp_wifi/include -I/Users/oak/esp/esp-idf/components/esp_event/include -I/Users/oak/esp/esp-idf/components/esp_netif/include -I/Users/oak/esp/esp-idf/components/esp_eth/include -I/Users/oak/esp/esp-idf/components/tcpip_adapter/include -I/Users/oak/esp/esp-idf/components/esp_phy/include -I/Users/oak/esp/esp-idf/components/esp_phy/esp32/include -I/Users/oak/esp/esp-idf/components/app_trace/include -I/Users/oak/esp/esp-idf/components/esp_timer/include -I/Users/oak/esp/esp-idf/components/mbedtls/port/include -I/Users/oak/esp/esp-idf/components/mbedtls/mbedtls/include -I/Users/oak/esp/esp-idf/components/mbedtls/esp_crt_bundle/include

ASM_FLAGS = -g -ffunction-sections -fdata-sections -Wall -Werror=all -Wno-error=unused-function -Wno-error=unused-variable -Wno-error=deprecated-declarations -Wextra -Wno-unused-parameter -Wno-sign-compare -ggdb -Og -fmacro-prefix-map=/Users/oak/Development/erou-firmware-esp32=. -fmacro-prefix-map=/Users/oak/esp/esp-idf=IDF -fstrict-volatile-bitfields -Wno-error=unused-but-set-variable -D_GNU_SOURCE -DIDF_VER=\"v4.4-dev-1594-g1d7068e4b\" -DESP_PLATFORM -Wno-unused-function

C_DEFINES = -DCONFIGURED -DHAVE_WEAK_SYMBOLS -DMBEDTLS_CONFIG_FILE=\"mbedtls/esp_config.h\" -DNATIVE_LITTLE_ENDIAN -D__STDC_CONSTANT_MACROS -D__STDC_LIMIT_MACROS

C_INCLUDES = -I/Users/oak/Development/erou-firmware-esp32/cmake-build-debug/config -I/Users/oak/esp/esp-idf/components/libsodium/libsodium/src/libsodium/include -I/Users/oak/esp/esp-idf/components/libsodium/port_include -I/Users/oak/esp/esp-idf/components/libsodium/libsodium/src/libsodium/include/sodium -I/Users/oak/esp/esp-idf/components/libsodium/port_include/sodium -I/Users/oak/esp/esp-idf/components/libsodium/port -I/Users/oak/esp/esp-idf/components/newlib/platform_include -I/Users/oak/esp/esp-idf/components/freertos/include -I/Users/oak/esp/esp-idf/components/freertos/port/xtensa/include -I/Users/oak/esp/esp-idf/components/esp_hw_support/include -I/Users/oak/esp/esp-idf/components/esp_hw_support/include/soc -I/Users/oak/esp/esp-idf/components/esp_hw_support/port/esp32/. -I/Users/oak/esp/esp-idf/components/heap/include -I/Users/oak/esp/esp-idf/components/log/include -I/Users/oak/esp/esp-idf/components/lwip/include/apps -I/Users/oak/esp/esp-idf/components/lwip/include/apps/sntp -I/Users/oak/esp/esp-idf/components/lwip/lwip/src/include -I/Users/oak/esp/esp-idf/components/lwip/port/esp32/include -I/Users/oak/esp/esp-idf/components/lwip/port/esp32/include/arch -I/Users/oak/esp/esp-idf/components/soc/include -I/Users/oak/esp/esp-idf/components/soc/esp32/. -I/Users/oak/esp/esp-idf/components/soc/esp32/include -I/Users/oak/esp/esp-idf/components/hal/esp32/include -I/Users/oak/esp/esp-idf/components/hal/include -I/Users/oak/esp/esp-idf/components/esp_rom/include -I/Users/oak/esp/esp-idf/components/esp_rom/esp32 -I/Users/oak/esp/esp-idf/components/esp_common/include -I/Users/oak/esp/esp-idf/components/esp_system/include -I/Users/oak/esp/esp-idf/components/esp_system/port/soc -I/Users/oak/esp/esp-idf/components/esp_system/port/public_compat -I/Users/oak/esp/esp-idf/components/esp32/include -I/Users/oak/esp/esp-idf/components/driver/include -I/Users/oak/esp/esp-idf/components/driver/esp32/include -I/Users/oak/esp/esp-idf/components/esp_pm/include -I/Users/oak/esp/esp-idf/components/esp_ringbuf/include -I/Users/oak/esp/esp-idf/components/efuse/include -I/Users/oak/esp/esp-idf/components/efuse/esp32/include -I/Users/oak/esp/esp-idf/components/xtensa/include -I/Users/oak/esp/esp-idf/components/xtensa/esp32/include -I/Users/oak/esp/esp-idf/components/vfs/include -I/Users/oak/esp/esp-idf/components/esp_wifi/include -I/Users/oak/esp/esp-idf/components/esp_event/include -I/Users/oak/esp/esp-idf/components/esp_netif/include -I/Users/oak/esp/esp-idf/components/esp_eth/include -I/Users/oak/esp/esp-idf/components/tcpip_adapter/include -I/Users/oak/esp/esp-idf/components/esp_phy/include -I/Users/oak/esp/esp-idf/components/esp_phy/esp32/include -I/Users/oak/esp/esp-idf/components/app_trace/include -I/Users/oak/esp/esp-idf/components/esp_timer/include -I/Users/oak/esp/esp-idf/components/mbedtls/port/include -I/Users/oak/esp/esp-idf/components/mbedtls/mbedtls/include -I/Users/oak/esp/esp-idf/components/mbedtls/esp_crt_bundle/include

C_FLAGS = -mlongcalls -Wno-frame-address -g -ffunction-sections -fdata-sections -Wall -Werror=all -Wno-error=unused-function -Wno-error=unused-variable -Wno-error=deprecated-declarations -Wextra -Wno-unused-parameter -Wno-sign-compare -ggdb -Og -fmacro-prefix-map=/Users/oak/Development/erou-firmware-esp32=. -fmacro-prefix-map=/Users/oak/esp/esp-idf=IDF -fstrict-volatile-bitfields -Wno-error=unused-but-set-variable -std=gnu99 -Wno-old-style-declaration -D_GNU_SOURCE -DIDF_VER=\"v4.4-dev-1594-g1d7068e4b\" -DESP_PLATFORM -Wno-unused-function

# Custom flags: esp-idf/libsodium/CMakeFiles/__idf_libsodium.dir/libsodium/src/libsodium/crypto_pwhash/argon2/argon2-core.c.obj_FLAGS = -Wno-type-limits

# Custom flags: esp-idf/libsodium/CMakeFiles/__idf_libsodium.dir/libsodium/src/libsodium/crypto_pwhash/argon2/argon2-fill-block-ref.c.obj_FLAGS = -Wno-unknown-pragmas

# Custom flags: esp-idf/libsodium/CMakeFiles/__idf_libsodium.dir/libsodium/src/libsodium/crypto_pwhash/argon2/pwhash_argon2i.c.obj_FLAGS = -Wno-type-limits

# Custom flags: esp-idf/libsodium/CMakeFiles/__idf_libsodium.dir/libsodium/src/libsodium/crypto_pwhash/argon2/pwhash_argon2id.c.obj_FLAGS = -Wno-type-limits

# Custom flags: esp-idf/libsodium/CMakeFiles/__idf_libsodium.dir/libsodium/src/libsodium/crypto_pwhash/scryptsalsa208sha256/pwhash_scryptsalsa208sha256.c.obj_FLAGS = -Wno-type-limits

# Custom flags: esp-idf/libsodium/CMakeFiles/__idf_libsodium.dir/libsodium/src/libsodium/crypto_shorthash/siphash24/ref/shorthash_siphash24_ref.c.obj_FLAGS = -Wno-implicit-fallthrough

# Custom flags: esp-idf/libsodium/CMakeFiles/__idf_libsodium.dir/libsodium/src/libsodium/crypto_shorthash/siphash24/ref/shorthash_siphashx24_ref.c.obj_FLAGS = -Wno-implicit-fallthrough

# Custom flags: esp-idf/libsodium/CMakeFiles/__idf_libsodium.dir/libsodium/src/libsodium/randombytes/randombytes.c.obj_FLAGS = -DRANDOMBYTES_DEFAULT_IMPLEMENTATION

# Custom flags: esp-idf/libsodium/CMakeFiles/__idf_libsodium.dir/libsodium/src/libsodium/sodium/utils.c.obj_FLAGS = -Wno-unused-variable

