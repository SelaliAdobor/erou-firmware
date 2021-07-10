import os.path
from glob import glob
from os.path import join

libDir = "./components/external-libraries"
srcFiles = []
srcExclusions = [
    f'ESPAsyncWebServer/extras',
    f'ESPAsyncWebServer/examples',
    "binn/examples",
    "binn/conversions",
    "binn/src/win32",
    "binn/test",
    "build",
    "croncpp/benchmark"
    , "croncpp/catch"
    , "croncpp/res",
    "croncpp/test"]

# Special case for fmt::fmt since it requires including parent dir
includeDirs = ["fmt-esp32"]
includeExclusions = ["fmt-esp32/fmt"]

print(f'{libDir}/**')

for ext in ('*.c', '*.cpp', '*.cc', '*.cxx'):
    srcFiles.extend(map(lambda path: os.path.relpath(path, libDir), glob(join(f'{libDir}/**/*', ext), recursive=True)))

for ext in ('*.h', '*.hpp'):
    includeDirs.extend(map(lambda path: os.path.relpath(os.path.dirname(path), libDir),
                           glob(join(f'{libDir}/**/*', ext), recursive=True)))

includeDirs = (list(set(includeDirs)))

includeDirs = filter(lambda i: list(filter(i.startswith, includeExclusions)) == [], includeDirs)
srcFiles = filter(lambda i: list(filter(i.startswith, srcExclusions)) == [], srcFiles)

print(srcFiles)
print(includeDirs)

srcFilesFormatted = " ".join(f'"{s}"' for s in srcFiles)
includeDirsFormatted = " ".join(f'"{i}"' for i in includeDirs)

cmakeContent = "idf_component_register(\n"
cmakeContent += f'  SRCS {srcFilesFormatted}\n'
cmakeContent += f'  INCLUDE_DIRS {includeDirsFormatted}\n'
cmakeContent += f'  REQUIRES arduino\n'
cmakeContent += f')'
cmakeContent += "\n"
cmakeContent += "set_target_properties(${COMPONENT_TARGET} PROPERTIES LINK_INTERFACE_MULTIPLICITY 3)"
print(cmakeContent)

cmakeFilePath = os.path.join(libDir, "CMakeLists.txt")

f = open(cmakeFilePath, "w")
f.write(cmakeContent)
f.close()
