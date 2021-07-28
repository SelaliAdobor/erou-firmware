import os.path
from glob import glob
from os.path import join

libDir = "./components/external-libraries"
srcFiles = [
    "cJSON/cJSON.c",
    "cJSON/cJSON_Utils.c",
    "mongoose/mongoose.c",
    "libyuarel/yuarel.c",
    "ccronexpr/ccronexpr.c"
]
includeRoots = [
    f'ESPAsyncWebServer/extras',
]
libraryRequires = [
    "arduino-esp32",
]
# Special case for fmt::fmt since it requires including parent dir
includeDirs = [
    "cJSON",
    "mongoose",
    "libyuarel",
    "ccronexpr"
]
srcRoots = [
    "TMCStepper/src",
    "ESP-FlexyStepper/src",
    "AsyncTCP/src",
    "ESPAsyncWebServer/src",
    "mjson/src"
]

for srcRoot in srcRoots:
    for srcExt in ('*.c', '*.cpp', '*.cc', '*.cxx'):
        foundFiles = glob(f'{libDir}/{srcRoot}/**/{srcExt}', recursive=True)
        relativeFiles = map(lambda path: os.path.relpath(path, libDir), foundFiles)
        srcFiles.extend(relativeFiles)
    for incExt in ('*.h', '*.hpp'):
        foundFiles = glob(f'{libDir}/{srcRoot}/**/{incExt}', recursive=True)
        relativeFiles = map(lambda path: os.path.relpath(path, libDir), foundFiles)
        relativeFiles = map(lambda path: os.path.dirname(path), relativeFiles)
        includeDirs.extend(relativeFiles)

includeDirs = (list(set(includeDirs)))

srcFilesFormatted = " ".join(f'"{s}"' for s in srcFiles)
includeDirsFormatted = " ".join(f'"{i}"' for i in includeDirs)
libraryRequiresFormatted = " ".join(f'"{i}"' for i in libraryRequires)

cmakeContent = "idf_component_register(\n"
cmakeContent += f'  SRCS {srcFilesFormatted}\n'
cmakeContent += f'  INCLUDE_DIRS {includeDirsFormatted}\n'
cmakeContent += f'  REQUIRES {libraryRequiresFormatted}\n'
cmakeContent += f')'
cmakeContent += "\n"
cmakeContent += "set_target_properties(${COMPONENT_TARGET} PROPERTIES LINK_INTERFACE_MULTIPLICITY 3)"
print(cmakeContent)

cmakeFilePath = os.path.join(libDir, "CMakeLists.txt")

f = open(cmakeFilePath, "w")
f.write(cmakeContent)
f.close()
