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

# Utility rule file for app_check_size.

# Include the progress variables for this target.
include esp-idf/esptool_py/CMakeFiles/app_check_size.dir/progress.make

esp-idf/esptool_py/CMakeFiles/app_check_size:
	cd /Users/oak/Development/erou-firmware-esp32/cmake-build-debug/esp-idf/esptool_py && python /Users/oak/esp/esp-idf/components/partition_table/check_sizes.py --offset 0x8000 partition --type app /Users/oak/Development/erou-firmware-esp32/cmake-build-debug/partition_table/partition-table.bin /Users/oak/Development/erou-firmware-esp32/cmake-build-debug/erou-firmware-esp32.bin

app_check_size: esp-idf/esptool_py/CMakeFiles/app_check_size
app_check_size: esp-idf/esptool_py/CMakeFiles/app_check_size.dir/build.make

.PHONY : app_check_size

# Rule to build all files generated by this target.
esp-idf/esptool_py/CMakeFiles/app_check_size.dir/build: app_check_size

.PHONY : esp-idf/esptool_py/CMakeFiles/app_check_size.dir/build

esp-idf/esptool_py/CMakeFiles/app_check_size.dir/clean:
	cd /Users/oak/Development/erou-firmware-esp32/cmake-build-debug/esp-idf/esptool_py && $(CMAKE_COMMAND) -P CMakeFiles/app_check_size.dir/cmake_clean.cmake
.PHONY : esp-idf/esptool_py/CMakeFiles/app_check_size.dir/clean

esp-idf/esptool_py/CMakeFiles/app_check_size.dir/depend:
	cd /Users/oak/Development/erou-firmware-esp32/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/oak/Development/erou-firmware-esp32 /Users/oak/esp/esp-idf/components/esptool_py /Users/oak/Development/erou-firmware-esp32/cmake-build-debug /Users/oak/Development/erou-firmware-esp32/cmake-build-debug/esp-idf/esptool_py /Users/oak/Development/erou-firmware-esp32/cmake-build-debug/esp-idf/esptool_py/CMakeFiles/app_check_size.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : esp-idf/esptool_py/CMakeFiles/app_check_size.dir/depend

