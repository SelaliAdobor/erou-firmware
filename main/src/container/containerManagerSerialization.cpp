#include "containerManager.h"
#include "wsdebug.h"
#include "ArduinoJson.h"
#include "SPIFFS.h"

void ContainerManager::loadFromDisk() {
  static_assert(config::physical::containerCount < 999,
                "Buffer allocation assumes max container count is less than 999..."
                "also that's a lot of containers.");
  if (!SPIFFS.exists(dbPath)) {
    debugE("No container.db defined on load.");
    return;
  }

  //Allocate a buffer for the entire DB and read out each item
  File containerDb = SPIFFS.open(dbPath);

  size_t dbSize = containerDb.size();

  auto dbBuffer = std::make_unique<unsigned char[]>(dbSize);
  auto read = containerDb.read(dbBuffer.get(), dbSize);
  debugV("Read %d bytes of %d from container DB", read, dbSize);
  containerDb.close();
  DynamicJsonDocument doc(dbSize);

  deserializeJson(doc, dbBuffer.get(), dbSize);

  JsonObject containers = doc[jsonContainerKey];
  uint32_t version = doc[jsonVersionKey];
  if (version != Container::currentVersion) {
    debugE("Version saved to disk does not match current version, disk(%d) current(%d)",
           version,
           Container::currentVersion);
    //todo: Manage migrations
  }

  for (int index = 0; index < config::physical::containerCount; index++) {
    JsonObject serialized = containers[String(index)];
    if (serialized.isNull()) {
      debugV("No container saved found %d", index);
      hasContent[index] = false;
      continue;
    }
    debugV("Found saved container %d", index);

    Container content;
    content.deserializeFrom(serialized);
    containerContents[index] = content;
    hasContent[index] = true;
  }
}

void ContainerManager::writeToDisk() {
  DynamicJsonDocument doc(maxContainerDBSize);
  auto root = doc.as<JsonObject>();

  root[jsonVersionKey] = Container::currentVersion;

  JsonObject containers = root.createNestedObject(jsonContainerKey);
  for (int index = 0; index < config::physical::containerCount; index++) {
    if (!hasContent[index]) {
      containers[String(index)] = nullptr;
      continue;
    }
    JsonObject container = containers.createNestedObject(String(index));
    containerContents[index].serializeInto(container);
    containers[String(index)] = container;
  }
  size_t docSize = measureJson(doc);

  char jsonBuffer[docSize];
  serializeJson(doc, jsonBuffer, docSize);

  updateDbFile(jsonBuffer, docSize);

  debugV("Container definition size written to disk: %d", docSize);
}

void ContainerManager::updateDbFile(char *buffer, size_t length) {
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
