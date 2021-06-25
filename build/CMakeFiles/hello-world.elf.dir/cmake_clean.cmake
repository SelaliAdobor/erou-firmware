file(REMOVE_RECURSE
  "bootloader/bootloader.bin"
  "bootloader/bootloader.elf"
  "bootloader/bootloader.map"
  "config/sdkconfig.cmake"
  "config/sdkconfig.h"
  "flash_project_args"
  "hello-world.bin"
  "hello-world.map"
  "project_elf_src_esp32.c"
  "CMakeFiles/hello-world.elf.dir/project_elf_src_esp32.c.obj"
  "CMakeFiles/hello-world.elf.dir/project_elf_src_esp32.c.obj.d"
  "hello-world.elf"
  "hello-world.elf.pdb"
  "project_elf_src_esp32.c"
)

# Per-language clean rules from dependency scanning.
foreach(lang C)
  include(CMakeFiles/hello-world.elf.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
