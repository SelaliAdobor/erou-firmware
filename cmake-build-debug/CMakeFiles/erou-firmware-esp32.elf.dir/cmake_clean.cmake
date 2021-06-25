file(REMOVE_RECURSE
  "bootloader/bootloader.bin"
  "bootloader/bootloader.elf"
  "bootloader/bootloader.map"
  "config/sdkconfig.cmake"
  "config/sdkconfig.h"
  "erou-firmware-esp32.bin"
  "erou-firmware-esp32.map"
  "flash_project_args"
  "project_elf_src_esp32.c"
  "CMakeFiles/erou-firmware-esp32.elf.dir/project_elf_src_esp32.c.obj"
  "erou-firmware-esp32.elf"
  "erou-firmware-esp32.elf.pdb"
  "project_elf_src_esp32.c"
)

# Per-language clean rules from dependency scanning.
foreach(lang C)
  include(CMakeFiles/erou-firmware-esp32.elf.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
