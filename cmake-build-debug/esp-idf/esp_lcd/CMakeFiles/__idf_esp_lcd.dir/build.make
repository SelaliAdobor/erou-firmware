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

# Include any dependencies generated for this target.
include esp-idf/esp_lcd/CMakeFiles/__idf_esp_lcd.dir/depend.make

# Include the progress variables for this target.
include esp-idf/esp_lcd/CMakeFiles/__idf_esp_lcd.dir/progress.make

# Include the compile flags for this target's objects.
include esp-idf/esp_lcd/CMakeFiles/__idf_esp_lcd.dir/flags.make

esp-idf/esp_lcd/CMakeFiles/__idf_esp_lcd.dir/src/esp_lcd_common.c.obj: esp-idf/esp_lcd/CMakeFiles/__idf_esp_lcd.dir/flags.make
esp-idf/esp_lcd/CMakeFiles/__idf_esp_lcd.dir/src/esp_lcd_common.c.obj: /Users/oak/esp/esp-idf/components/esp_lcd/src/esp_lcd_common.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/oak/Development/erou-firmware-esp32/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object esp-idf/esp_lcd/CMakeFiles/__idf_esp_lcd.dir/src/esp_lcd_common.c.obj"
	cd /Users/oak/Development/erou-firmware-esp32/cmake-build-debug/esp-idf/esp_lcd && /Users/oak/.espressif/tools/xtensa-esp32-elf/esp-2020r3-8.4.0/xtensa-esp32-elf/bin/xtensa-esp32-elf-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/__idf_esp_lcd.dir/src/esp_lcd_common.c.obj -c /Users/oak/esp/esp-idf/components/esp_lcd/src/esp_lcd_common.c

esp-idf/esp_lcd/CMakeFiles/__idf_esp_lcd.dir/src/esp_lcd_common.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/__idf_esp_lcd.dir/src/esp_lcd_common.c.i"
	cd /Users/oak/Development/erou-firmware-esp32/cmake-build-debug/esp-idf/esp_lcd && /Users/oak/.espressif/tools/xtensa-esp32-elf/esp-2020r3-8.4.0/xtensa-esp32-elf/bin/xtensa-esp32-elf-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/oak/esp/esp-idf/components/esp_lcd/src/esp_lcd_common.c > CMakeFiles/__idf_esp_lcd.dir/src/esp_lcd_common.c.i

esp-idf/esp_lcd/CMakeFiles/__idf_esp_lcd.dir/src/esp_lcd_common.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/__idf_esp_lcd.dir/src/esp_lcd_common.c.s"
	cd /Users/oak/Development/erou-firmware-esp32/cmake-build-debug/esp-idf/esp_lcd && /Users/oak/.espressif/tools/xtensa-esp32-elf/esp-2020r3-8.4.0/xtensa-esp32-elf/bin/xtensa-esp32-elf-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/oak/esp/esp-idf/components/esp_lcd/src/esp_lcd_common.c -o CMakeFiles/__idf_esp_lcd.dir/src/esp_lcd_common.c.s

esp-idf/esp_lcd/CMakeFiles/__idf_esp_lcd.dir/src/esp_lcd_panel_io.c.obj: esp-idf/esp_lcd/CMakeFiles/__idf_esp_lcd.dir/flags.make
esp-idf/esp_lcd/CMakeFiles/__idf_esp_lcd.dir/src/esp_lcd_panel_io.c.obj: /Users/oak/esp/esp-idf/components/esp_lcd/src/esp_lcd_panel_io.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/oak/Development/erou-firmware-esp32/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object esp-idf/esp_lcd/CMakeFiles/__idf_esp_lcd.dir/src/esp_lcd_panel_io.c.obj"
	cd /Users/oak/Development/erou-firmware-esp32/cmake-build-debug/esp-idf/esp_lcd && /Users/oak/.espressif/tools/xtensa-esp32-elf/esp-2020r3-8.4.0/xtensa-esp32-elf/bin/xtensa-esp32-elf-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/__idf_esp_lcd.dir/src/esp_lcd_panel_io.c.obj -c /Users/oak/esp/esp-idf/components/esp_lcd/src/esp_lcd_panel_io.c

esp-idf/esp_lcd/CMakeFiles/__idf_esp_lcd.dir/src/esp_lcd_panel_io.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/__idf_esp_lcd.dir/src/esp_lcd_panel_io.c.i"
	cd /Users/oak/Development/erou-firmware-esp32/cmake-build-debug/esp-idf/esp_lcd && /Users/oak/.espressif/tools/xtensa-esp32-elf/esp-2020r3-8.4.0/xtensa-esp32-elf/bin/xtensa-esp32-elf-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/oak/esp/esp-idf/components/esp_lcd/src/esp_lcd_panel_io.c > CMakeFiles/__idf_esp_lcd.dir/src/esp_lcd_panel_io.c.i

esp-idf/esp_lcd/CMakeFiles/__idf_esp_lcd.dir/src/esp_lcd_panel_io.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/__idf_esp_lcd.dir/src/esp_lcd_panel_io.c.s"
	cd /Users/oak/Development/erou-firmware-esp32/cmake-build-debug/esp-idf/esp_lcd && /Users/oak/.espressif/tools/xtensa-esp32-elf/esp-2020r3-8.4.0/xtensa-esp32-elf/bin/xtensa-esp32-elf-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/oak/esp/esp-idf/components/esp_lcd/src/esp_lcd_panel_io.c -o CMakeFiles/__idf_esp_lcd.dir/src/esp_lcd_panel_io.c.s

esp-idf/esp_lcd/CMakeFiles/__idf_esp_lcd.dir/src/esp_lcd_panel_io_i2c.c.obj: esp-idf/esp_lcd/CMakeFiles/__idf_esp_lcd.dir/flags.make
esp-idf/esp_lcd/CMakeFiles/__idf_esp_lcd.dir/src/esp_lcd_panel_io_i2c.c.obj: /Users/oak/esp/esp-idf/components/esp_lcd/src/esp_lcd_panel_io_i2c.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/oak/Development/erou-firmware-esp32/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object esp-idf/esp_lcd/CMakeFiles/__idf_esp_lcd.dir/src/esp_lcd_panel_io_i2c.c.obj"
	cd /Users/oak/Development/erou-firmware-esp32/cmake-build-debug/esp-idf/esp_lcd && /Users/oak/.espressif/tools/xtensa-esp32-elf/esp-2020r3-8.4.0/xtensa-esp32-elf/bin/xtensa-esp32-elf-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/__idf_esp_lcd.dir/src/esp_lcd_panel_io_i2c.c.obj -c /Users/oak/esp/esp-idf/components/esp_lcd/src/esp_lcd_panel_io_i2c.c

esp-idf/esp_lcd/CMakeFiles/__idf_esp_lcd.dir/src/esp_lcd_panel_io_i2c.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/__idf_esp_lcd.dir/src/esp_lcd_panel_io_i2c.c.i"
	cd /Users/oak/Development/erou-firmware-esp32/cmake-build-debug/esp-idf/esp_lcd && /Users/oak/.espressif/tools/xtensa-esp32-elf/esp-2020r3-8.4.0/xtensa-esp32-elf/bin/xtensa-esp32-elf-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/oak/esp/esp-idf/components/esp_lcd/src/esp_lcd_panel_io_i2c.c > CMakeFiles/__idf_esp_lcd.dir/src/esp_lcd_panel_io_i2c.c.i

esp-idf/esp_lcd/CMakeFiles/__idf_esp_lcd.dir/src/esp_lcd_panel_io_i2c.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/__idf_esp_lcd.dir/src/esp_lcd_panel_io_i2c.c.s"
	cd /Users/oak/Development/erou-firmware-esp32/cmake-build-debug/esp-idf/esp_lcd && /Users/oak/.espressif/tools/xtensa-esp32-elf/esp-2020r3-8.4.0/xtensa-esp32-elf/bin/xtensa-esp32-elf-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/oak/esp/esp-idf/components/esp_lcd/src/esp_lcd_panel_io_i2c.c -o CMakeFiles/__idf_esp_lcd.dir/src/esp_lcd_panel_io_i2c.c.s

esp-idf/esp_lcd/CMakeFiles/__idf_esp_lcd.dir/src/esp_lcd_panel_io_spi.c.obj: esp-idf/esp_lcd/CMakeFiles/__idf_esp_lcd.dir/flags.make
esp-idf/esp_lcd/CMakeFiles/__idf_esp_lcd.dir/src/esp_lcd_panel_io_spi.c.obj: /Users/oak/esp/esp-idf/components/esp_lcd/src/esp_lcd_panel_io_spi.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/oak/Development/erou-firmware-esp32/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building C object esp-idf/esp_lcd/CMakeFiles/__idf_esp_lcd.dir/src/esp_lcd_panel_io_spi.c.obj"
	cd /Users/oak/Development/erou-firmware-esp32/cmake-build-debug/esp-idf/esp_lcd && /Users/oak/.espressif/tools/xtensa-esp32-elf/esp-2020r3-8.4.0/xtensa-esp32-elf/bin/xtensa-esp32-elf-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/__idf_esp_lcd.dir/src/esp_lcd_panel_io_spi.c.obj -c /Users/oak/esp/esp-idf/components/esp_lcd/src/esp_lcd_panel_io_spi.c

esp-idf/esp_lcd/CMakeFiles/__idf_esp_lcd.dir/src/esp_lcd_panel_io_spi.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/__idf_esp_lcd.dir/src/esp_lcd_panel_io_spi.c.i"
	cd /Users/oak/Development/erou-firmware-esp32/cmake-build-debug/esp-idf/esp_lcd && /Users/oak/.espressif/tools/xtensa-esp32-elf/esp-2020r3-8.4.0/xtensa-esp32-elf/bin/xtensa-esp32-elf-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/oak/esp/esp-idf/components/esp_lcd/src/esp_lcd_panel_io_spi.c > CMakeFiles/__idf_esp_lcd.dir/src/esp_lcd_panel_io_spi.c.i

esp-idf/esp_lcd/CMakeFiles/__idf_esp_lcd.dir/src/esp_lcd_panel_io_spi.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/__idf_esp_lcd.dir/src/esp_lcd_panel_io_spi.c.s"
	cd /Users/oak/Development/erou-firmware-esp32/cmake-build-debug/esp-idf/esp_lcd && /Users/oak/.espressif/tools/xtensa-esp32-elf/esp-2020r3-8.4.0/xtensa-esp32-elf/bin/xtensa-esp32-elf-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/oak/esp/esp-idf/components/esp_lcd/src/esp_lcd_panel_io_spi.c -o CMakeFiles/__idf_esp_lcd.dir/src/esp_lcd_panel_io_spi.c.s

esp-idf/esp_lcd/CMakeFiles/__idf_esp_lcd.dir/src/esp_lcd_panel_io_i80.c.obj: esp-idf/esp_lcd/CMakeFiles/__idf_esp_lcd.dir/flags.make
esp-idf/esp_lcd/CMakeFiles/__idf_esp_lcd.dir/src/esp_lcd_panel_io_i80.c.obj: /Users/oak/esp/esp-idf/components/esp_lcd/src/esp_lcd_panel_io_i80.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/oak/Development/erou-firmware-esp32/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building C object esp-idf/esp_lcd/CMakeFiles/__idf_esp_lcd.dir/src/esp_lcd_panel_io_i80.c.obj"
	cd /Users/oak/Development/erou-firmware-esp32/cmake-build-debug/esp-idf/esp_lcd && /Users/oak/.espressif/tools/xtensa-esp32-elf/esp-2020r3-8.4.0/xtensa-esp32-elf/bin/xtensa-esp32-elf-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/__idf_esp_lcd.dir/src/esp_lcd_panel_io_i80.c.obj -c /Users/oak/esp/esp-idf/components/esp_lcd/src/esp_lcd_panel_io_i80.c

esp-idf/esp_lcd/CMakeFiles/__idf_esp_lcd.dir/src/esp_lcd_panel_io_i80.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/__idf_esp_lcd.dir/src/esp_lcd_panel_io_i80.c.i"
	cd /Users/oak/Development/erou-firmware-esp32/cmake-build-debug/esp-idf/esp_lcd && /Users/oak/.espressif/tools/xtensa-esp32-elf/esp-2020r3-8.4.0/xtensa-esp32-elf/bin/xtensa-esp32-elf-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/oak/esp/esp-idf/components/esp_lcd/src/esp_lcd_panel_io_i80.c > CMakeFiles/__idf_esp_lcd.dir/src/esp_lcd_panel_io_i80.c.i

esp-idf/esp_lcd/CMakeFiles/__idf_esp_lcd.dir/src/esp_lcd_panel_io_i80.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/__idf_esp_lcd.dir/src/esp_lcd_panel_io_i80.c.s"
	cd /Users/oak/Development/erou-firmware-esp32/cmake-build-debug/esp-idf/esp_lcd && /Users/oak/.espressif/tools/xtensa-esp32-elf/esp-2020r3-8.4.0/xtensa-esp32-elf/bin/xtensa-esp32-elf-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/oak/esp/esp-idf/components/esp_lcd/src/esp_lcd_panel_io_i80.c -o CMakeFiles/__idf_esp_lcd.dir/src/esp_lcd_panel_io_i80.c.s

esp-idf/esp_lcd/CMakeFiles/__idf_esp_lcd.dir/src/esp_lcd_panel_ssd1306.c.obj: esp-idf/esp_lcd/CMakeFiles/__idf_esp_lcd.dir/flags.make
esp-idf/esp_lcd/CMakeFiles/__idf_esp_lcd.dir/src/esp_lcd_panel_ssd1306.c.obj: /Users/oak/esp/esp-idf/components/esp_lcd/src/esp_lcd_panel_ssd1306.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/oak/Development/erou-firmware-esp32/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building C object esp-idf/esp_lcd/CMakeFiles/__idf_esp_lcd.dir/src/esp_lcd_panel_ssd1306.c.obj"
	cd /Users/oak/Development/erou-firmware-esp32/cmake-build-debug/esp-idf/esp_lcd && /Users/oak/.espressif/tools/xtensa-esp32-elf/esp-2020r3-8.4.0/xtensa-esp32-elf/bin/xtensa-esp32-elf-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/__idf_esp_lcd.dir/src/esp_lcd_panel_ssd1306.c.obj -c /Users/oak/esp/esp-idf/components/esp_lcd/src/esp_lcd_panel_ssd1306.c

esp-idf/esp_lcd/CMakeFiles/__idf_esp_lcd.dir/src/esp_lcd_panel_ssd1306.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/__idf_esp_lcd.dir/src/esp_lcd_panel_ssd1306.c.i"
	cd /Users/oak/Development/erou-firmware-esp32/cmake-build-debug/esp-idf/esp_lcd && /Users/oak/.espressif/tools/xtensa-esp32-elf/esp-2020r3-8.4.0/xtensa-esp32-elf/bin/xtensa-esp32-elf-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/oak/esp/esp-idf/components/esp_lcd/src/esp_lcd_panel_ssd1306.c > CMakeFiles/__idf_esp_lcd.dir/src/esp_lcd_panel_ssd1306.c.i

esp-idf/esp_lcd/CMakeFiles/__idf_esp_lcd.dir/src/esp_lcd_panel_ssd1306.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/__idf_esp_lcd.dir/src/esp_lcd_panel_ssd1306.c.s"
	cd /Users/oak/Development/erou-firmware-esp32/cmake-build-debug/esp-idf/esp_lcd && /Users/oak/.espressif/tools/xtensa-esp32-elf/esp-2020r3-8.4.0/xtensa-esp32-elf/bin/xtensa-esp32-elf-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/oak/esp/esp-idf/components/esp_lcd/src/esp_lcd_panel_ssd1306.c -o CMakeFiles/__idf_esp_lcd.dir/src/esp_lcd_panel_ssd1306.c.s

esp-idf/esp_lcd/CMakeFiles/__idf_esp_lcd.dir/src/esp_lcd_panel_st7789.c.obj: esp-idf/esp_lcd/CMakeFiles/__idf_esp_lcd.dir/flags.make
esp-idf/esp_lcd/CMakeFiles/__idf_esp_lcd.dir/src/esp_lcd_panel_st7789.c.obj: /Users/oak/esp/esp-idf/components/esp_lcd/src/esp_lcd_panel_st7789.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/oak/Development/erou-firmware-esp32/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building C object esp-idf/esp_lcd/CMakeFiles/__idf_esp_lcd.dir/src/esp_lcd_panel_st7789.c.obj"
	cd /Users/oak/Development/erou-firmware-esp32/cmake-build-debug/esp-idf/esp_lcd && /Users/oak/.espressif/tools/xtensa-esp32-elf/esp-2020r3-8.4.0/xtensa-esp32-elf/bin/xtensa-esp32-elf-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/__idf_esp_lcd.dir/src/esp_lcd_panel_st7789.c.obj -c /Users/oak/esp/esp-idf/components/esp_lcd/src/esp_lcd_panel_st7789.c

esp-idf/esp_lcd/CMakeFiles/__idf_esp_lcd.dir/src/esp_lcd_panel_st7789.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/__idf_esp_lcd.dir/src/esp_lcd_panel_st7789.c.i"
	cd /Users/oak/Development/erou-firmware-esp32/cmake-build-debug/esp-idf/esp_lcd && /Users/oak/.espressif/tools/xtensa-esp32-elf/esp-2020r3-8.4.0/xtensa-esp32-elf/bin/xtensa-esp32-elf-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/oak/esp/esp-idf/components/esp_lcd/src/esp_lcd_panel_st7789.c > CMakeFiles/__idf_esp_lcd.dir/src/esp_lcd_panel_st7789.c.i

esp-idf/esp_lcd/CMakeFiles/__idf_esp_lcd.dir/src/esp_lcd_panel_st7789.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/__idf_esp_lcd.dir/src/esp_lcd_panel_st7789.c.s"
	cd /Users/oak/Development/erou-firmware-esp32/cmake-build-debug/esp-idf/esp_lcd && /Users/oak/.espressif/tools/xtensa-esp32-elf/esp-2020r3-8.4.0/xtensa-esp32-elf/bin/xtensa-esp32-elf-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/oak/esp/esp-idf/components/esp_lcd/src/esp_lcd_panel_st7789.c -o CMakeFiles/__idf_esp_lcd.dir/src/esp_lcd_panel_st7789.c.s

esp-idf/esp_lcd/CMakeFiles/__idf_esp_lcd.dir/src/esp_lcd_panel_ops.c.obj: esp-idf/esp_lcd/CMakeFiles/__idf_esp_lcd.dir/flags.make
esp-idf/esp_lcd/CMakeFiles/__idf_esp_lcd.dir/src/esp_lcd_panel_ops.c.obj: /Users/oak/esp/esp-idf/components/esp_lcd/src/esp_lcd_panel_ops.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/oak/Development/erou-firmware-esp32/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building C object esp-idf/esp_lcd/CMakeFiles/__idf_esp_lcd.dir/src/esp_lcd_panel_ops.c.obj"
	cd /Users/oak/Development/erou-firmware-esp32/cmake-build-debug/esp-idf/esp_lcd && /Users/oak/.espressif/tools/xtensa-esp32-elf/esp-2020r3-8.4.0/xtensa-esp32-elf/bin/xtensa-esp32-elf-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/__idf_esp_lcd.dir/src/esp_lcd_panel_ops.c.obj -c /Users/oak/esp/esp-idf/components/esp_lcd/src/esp_lcd_panel_ops.c

esp-idf/esp_lcd/CMakeFiles/__idf_esp_lcd.dir/src/esp_lcd_panel_ops.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/__idf_esp_lcd.dir/src/esp_lcd_panel_ops.c.i"
	cd /Users/oak/Development/erou-firmware-esp32/cmake-build-debug/esp-idf/esp_lcd && /Users/oak/.espressif/tools/xtensa-esp32-elf/esp-2020r3-8.4.0/xtensa-esp32-elf/bin/xtensa-esp32-elf-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/oak/esp/esp-idf/components/esp_lcd/src/esp_lcd_panel_ops.c > CMakeFiles/__idf_esp_lcd.dir/src/esp_lcd_panel_ops.c.i

esp-idf/esp_lcd/CMakeFiles/__idf_esp_lcd.dir/src/esp_lcd_panel_ops.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/__idf_esp_lcd.dir/src/esp_lcd_panel_ops.c.s"
	cd /Users/oak/Development/erou-firmware-esp32/cmake-build-debug/esp-idf/esp_lcd && /Users/oak/.espressif/tools/xtensa-esp32-elf/esp-2020r3-8.4.0/xtensa-esp32-elf/bin/xtensa-esp32-elf-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/oak/esp/esp-idf/components/esp_lcd/src/esp_lcd_panel_ops.c -o CMakeFiles/__idf_esp_lcd.dir/src/esp_lcd_panel_ops.c.s

esp-idf/esp_lcd/CMakeFiles/__idf_esp_lcd.dir/src/esp_lcd_rgb_panel.c.obj: esp-idf/esp_lcd/CMakeFiles/__idf_esp_lcd.dir/flags.make
esp-idf/esp_lcd/CMakeFiles/__idf_esp_lcd.dir/src/esp_lcd_rgb_panel.c.obj: /Users/oak/esp/esp-idf/components/esp_lcd/src/esp_lcd_rgb_panel.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/oak/Development/erou-firmware-esp32/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building C object esp-idf/esp_lcd/CMakeFiles/__idf_esp_lcd.dir/src/esp_lcd_rgb_panel.c.obj"
	cd /Users/oak/Development/erou-firmware-esp32/cmake-build-debug/esp-idf/esp_lcd && /Users/oak/.espressif/tools/xtensa-esp32-elf/esp-2020r3-8.4.0/xtensa-esp32-elf/bin/xtensa-esp32-elf-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/__idf_esp_lcd.dir/src/esp_lcd_rgb_panel.c.obj -c /Users/oak/esp/esp-idf/components/esp_lcd/src/esp_lcd_rgb_panel.c

esp-idf/esp_lcd/CMakeFiles/__idf_esp_lcd.dir/src/esp_lcd_rgb_panel.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/__idf_esp_lcd.dir/src/esp_lcd_rgb_panel.c.i"
	cd /Users/oak/Development/erou-firmware-esp32/cmake-build-debug/esp-idf/esp_lcd && /Users/oak/.espressif/tools/xtensa-esp32-elf/esp-2020r3-8.4.0/xtensa-esp32-elf/bin/xtensa-esp32-elf-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/oak/esp/esp-idf/components/esp_lcd/src/esp_lcd_rgb_panel.c > CMakeFiles/__idf_esp_lcd.dir/src/esp_lcd_rgb_panel.c.i

esp-idf/esp_lcd/CMakeFiles/__idf_esp_lcd.dir/src/esp_lcd_rgb_panel.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/__idf_esp_lcd.dir/src/esp_lcd_rgb_panel.c.s"
	cd /Users/oak/Development/erou-firmware-esp32/cmake-build-debug/esp-idf/esp_lcd && /Users/oak/.espressif/tools/xtensa-esp32-elf/esp-2020r3-8.4.0/xtensa-esp32-elf/bin/xtensa-esp32-elf-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/oak/esp/esp-idf/components/esp_lcd/src/esp_lcd_rgb_panel.c -o CMakeFiles/__idf_esp_lcd.dir/src/esp_lcd_rgb_panel.c.s

# Object files for target __idf_esp_lcd
__idf_esp_lcd_OBJECTS = \
"CMakeFiles/__idf_esp_lcd.dir/src/esp_lcd_common.c.obj" \
"CMakeFiles/__idf_esp_lcd.dir/src/esp_lcd_panel_io.c.obj" \
"CMakeFiles/__idf_esp_lcd.dir/src/esp_lcd_panel_io_i2c.c.obj" \
"CMakeFiles/__idf_esp_lcd.dir/src/esp_lcd_panel_io_spi.c.obj" \
"CMakeFiles/__idf_esp_lcd.dir/src/esp_lcd_panel_io_i80.c.obj" \
"CMakeFiles/__idf_esp_lcd.dir/src/esp_lcd_panel_ssd1306.c.obj" \
"CMakeFiles/__idf_esp_lcd.dir/src/esp_lcd_panel_st7789.c.obj" \
"CMakeFiles/__idf_esp_lcd.dir/src/esp_lcd_panel_ops.c.obj" \
"CMakeFiles/__idf_esp_lcd.dir/src/esp_lcd_rgb_panel.c.obj"

# External object files for target __idf_esp_lcd
__idf_esp_lcd_EXTERNAL_OBJECTS =

esp-idf/esp_lcd/libesp_lcd.a: esp-idf/esp_lcd/CMakeFiles/__idf_esp_lcd.dir/src/esp_lcd_common.c.obj
esp-idf/esp_lcd/libesp_lcd.a: esp-idf/esp_lcd/CMakeFiles/__idf_esp_lcd.dir/src/esp_lcd_panel_io.c.obj
esp-idf/esp_lcd/libesp_lcd.a: esp-idf/esp_lcd/CMakeFiles/__idf_esp_lcd.dir/src/esp_lcd_panel_io_i2c.c.obj
esp-idf/esp_lcd/libesp_lcd.a: esp-idf/esp_lcd/CMakeFiles/__idf_esp_lcd.dir/src/esp_lcd_panel_io_spi.c.obj
esp-idf/esp_lcd/libesp_lcd.a: esp-idf/esp_lcd/CMakeFiles/__idf_esp_lcd.dir/src/esp_lcd_panel_io_i80.c.obj
esp-idf/esp_lcd/libesp_lcd.a: esp-idf/esp_lcd/CMakeFiles/__idf_esp_lcd.dir/src/esp_lcd_panel_ssd1306.c.obj
esp-idf/esp_lcd/libesp_lcd.a: esp-idf/esp_lcd/CMakeFiles/__idf_esp_lcd.dir/src/esp_lcd_panel_st7789.c.obj
esp-idf/esp_lcd/libesp_lcd.a: esp-idf/esp_lcd/CMakeFiles/__idf_esp_lcd.dir/src/esp_lcd_panel_ops.c.obj
esp-idf/esp_lcd/libesp_lcd.a: esp-idf/esp_lcd/CMakeFiles/__idf_esp_lcd.dir/src/esp_lcd_rgb_panel.c.obj
esp-idf/esp_lcd/libesp_lcd.a: esp-idf/esp_lcd/CMakeFiles/__idf_esp_lcd.dir/build.make
esp-idf/esp_lcd/libesp_lcd.a: esp-idf/esp_lcd/CMakeFiles/__idf_esp_lcd.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/oak/Development/erou-firmware-esp32/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Linking C static library libesp_lcd.a"
	cd /Users/oak/Development/erou-firmware-esp32/cmake-build-debug/esp-idf/esp_lcd && $(CMAKE_COMMAND) -P CMakeFiles/__idf_esp_lcd.dir/cmake_clean_target.cmake
	cd /Users/oak/Development/erou-firmware-esp32/cmake-build-debug/esp-idf/esp_lcd && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/__idf_esp_lcd.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
esp-idf/esp_lcd/CMakeFiles/__idf_esp_lcd.dir/build: esp-idf/esp_lcd/libesp_lcd.a

.PHONY : esp-idf/esp_lcd/CMakeFiles/__idf_esp_lcd.dir/build

esp-idf/esp_lcd/CMakeFiles/__idf_esp_lcd.dir/clean:
	cd /Users/oak/Development/erou-firmware-esp32/cmake-build-debug/esp-idf/esp_lcd && $(CMAKE_COMMAND) -P CMakeFiles/__idf_esp_lcd.dir/cmake_clean.cmake
.PHONY : esp-idf/esp_lcd/CMakeFiles/__idf_esp_lcd.dir/clean

esp-idf/esp_lcd/CMakeFiles/__idf_esp_lcd.dir/depend:
	cd /Users/oak/Development/erou-firmware-esp32/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/oak/Development/erou-firmware-esp32 /Users/oak/esp/esp-idf/components/esp_lcd /Users/oak/Development/erou-firmware-esp32/cmake-build-debug /Users/oak/Development/erou-firmware-esp32/cmake-build-debug/esp-idf/esp_lcd /Users/oak/Development/erou-firmware-esp32/cmake-build-debug/esp-idf/esp_lcd/CMakeFiles/__idf_esp_lcd.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : esp-idf/esp_lcd/CMakeFiles/__idf_esp_lcd.dir/depend

