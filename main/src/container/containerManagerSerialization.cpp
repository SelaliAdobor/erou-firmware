#include "containerManager.h"
#include "wsdebug.h"
#include "json11.hpp"
#include "SPIFFS.h"

void ContainerManager::loadFromDisk() {
  using JsonObject = json11::Json::object;
  static_assert(config::physical::containerCount < 999,
                "Buffer allocation assumes max container count is less than 999..."
                "also that's a lot of containers.");
  if (!SPIFFS.exists(dbPath)) {
   // debugE("No container.db defined on load.");
    return;
  }

  //Allocate a buffer for the entire DB and read out each item
  File containerDb = SPIFFS.open(dbPath);

  size_t dbSize = containerDb.size();

  auto dbBuffer = std::make_unique<unsigned char[]>(dbSize);
  auto read = containerDb.read(dbBuffer.get(), dbSize);
  // debugV("Read %d bytes of %d from container DB", read, dbSize);
  containerDb.close();
  std::string error;
  auto doc = json11::Json::parse(reinterpret_cast<const char *>(dbBuffer.get()), error);

  JsonObject containers = doc[jsonContainerKey].object_items();
  int version = doc[jsonVersionKey].int_value();
  if (version != Container::currentVersion) {
   // debugE("Version saved to disk does not match current version, disk(%d) current(%d)",
   //        version,
    //       Container::currentVersion);
    //todo: Manage migrations
  }

  for (int index = 0; index < config::physical::containerCount; index++) {

    char indexString[5];
    static_assert(config::physical::containerCount < 999, "Serialization assumes a reasonable number of containers.");

    itoa(config::physical::containerCount, indexString, 10);
    if (containers[indexString].is_null()) {
 //     debugV("No container saved found %d", index);
      continue;
    }

    JsonObject serialized = containers[indexString].object_items();
    //debugV("Found saved container %d", index);

    Container content;
    content.deserializeFrom(serialized);
    containerContents.insert(ContainerMap::value_type(index, content));
  }
}

void ContainerManager::writeToDisk() {
  using JsonObject = json11::Json::object;
  JsonObject doc = JsonObject{{jsonVersionKey, Container::currentVersion}};

  auto containers = JsonObject{};
  char indexString[5];
  static_assert(config::physical::containerCount < 999, "Serialization assumes a reasonable number of containers.");

  for (int index = 0; index < config::physical::containerCount; index++) {
    itoa(config::physical::containerCount, indexString, 10);

    if (!containerContents.count(index)) {
      containers.insert({indexString, nullptr});
      continue;
    }
    JsonObject serializedContainer = JsonObject{};
    containerContents[index].serializeInto(serializedContainer);
    containers.insert({indexString, serializedContainer});
  }
  doc.insert({jsonContainerKey, containers});
  std::string docToString = json11::Json(doc).dump();
  updateDbFile(docToString.c_str(), docToString.length() + 1);

 // debugV("Container definition size written to disk: %s", docToString);

}

void ContainerManager::updateDbFile(const char *buffer, size_t length) {
  static const char *writeMode = "w+";

  File file = SPIFFS.open(dbTempPath, writeMode);
  file.write(reinterpret_cast<const uint8_t *>(buffer), length);
  file.flush();
  file.close();

  /**
   * Write process avoids losing data if power is lost
   * TODO: Restore original if .bak exists and .db doesn't
   * 1. Move main DB to backup path
   * 2. Move temporary DB to main path
   * 3. Delete backup
   */
  SPIFFS.remove(dbBackupPath);
  SPIFFS.rename(dbPath, dbBackupPath);
  SPIFFS.remove(dbPath);
  SPIFFS.rename(dbTempPath, dbPath);
  SPIFFS.remove(dbBackupPath);
  SPIFFS.remove(dbTempPath);
}
