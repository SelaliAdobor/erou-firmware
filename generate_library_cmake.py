import os.path
from glob import glob
from os.path import join

libDir = "./components/external-libraries"
srcFiles = []
srcExclusions = []

#Special case for fmt::fmt since it requires including parent dir
includeDirs = ["fmt-esp32"]
includeExclusions = ["fmt-esp32/fmt"]

for ext in ('*.cpp', '*.c', '*.cc', '*.cxx'):
    srcFiles.extend(map(lambda path: os.path.relpath(path, libDir), glob(join(f'{libDir}/**/**', ext))))

for ext in ('*.h', '*.hpp'):
    includeDirs.extend(map(lambda path: os.path.relpath(os.path.dirname(path), libDir), glob(join(f'{libDir}/**/**', ext))))

includeDirs = (list(set(includeDirs)))

includeDirs = filter(lambda i: i not in includeExclusions, includeDirs)
srcFiles = filter(lambda i: i not in srcExclusions, srcFiles)

print(srcFiles)
print(includeDirs)

srcFilesFormatted = " ".join(f'"{s}"' for s in srcFiles)
includeDirsFormatted = " ".join(f'"{i}"' for i in includeDirs)

cmakeContent = "idf_component_register(\n"
cmakeContent += f'  SRCS {srcFilesFormatted}\n'
cmakeContent += f'  INCLUDE_DIRS {includeDirsFormatted}\n'
cmakeContent += f'  REQUIRES arduino\n'
cmakeContent += f')'
print(cmakeContent)

cmakeFilePath = os.path.join(libDir, "CMakeLists.txt")

f = open(cmakeFilePath, "w")
f.write(cmakeContent)
f.close()
