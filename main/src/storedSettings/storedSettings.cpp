//
// Created by Selali Adobor on 7/13/21.
//

#include <wsdebug.h>
#include <SPIFFS.h>
#include "storedSettings.h"

StoredSettings storedSettings = StoredSettings();

void StoredSettings::setInt(const char *key, int value) {
  auto *property = cJSON_GetObjectItem(json, key);
  if (property != nullptr) {
    cJSON_ReplaceItemInObject(json, key, cJSON_CreateNumber(value));
  } else {
    cJSON_AddNumberToObject(json, key, static_cast<int>(value));
  }
  if (commitOnSet) {
    writeToDisk();
  }
}

void StoredSettings::setString(const char *key, const char *value) {
  auto *property = cJSON_GetObjectItem(json, key);
  if (property != nullptr) {
    cJSON_ReplaceItemInObject(json, key, cJSON_CreateString(value));
  } else {
    cJSON_AddStringToObject(json, key, value);
  }
  if (commitOnSet) {
    writeToDisk();
  }
}

void StoredSettings::setBool(const char *key, bool value) {
  setInt(key, static_cast<int>(value));
}

std::optional<int> StoredSettings::getInt(char *key) {
  auto property = cJSON_GetObjectItem(json, key);
  if (property == nullptr || cJSON_IsNull(property)) {
    return {};
  }
  double value = cJSON_GetNumberValue(property);
  return static_cast<int>(value);
}

std::optional<bool> StoredSettings::getBool(char *key) {
  debugI("Getting bool %s", key);
  auto property = cJSON_GetObjectItem(json, key);
  if (property == nullptr || cJSON_IsNull(property)) {
    return {};
  }
  double value = cJSON_GetNumberValue(property);
  debugI("Got bool %s %d", key, static_cast<int>(value));
  debugI("Got bool %s %d", key, (static_cast<int>(value) != 0));
  return static_cast<bool>(value);
}
std::optional<char *> StoredSettings::getString(char *key) {
  auto *property = cJSON_GetObjectItem(json, key);
  if (property == nullptr || cJSON_IsNull(property)) {
    return {};
  }
  char *value = cJSON_GetStringValue(property);
  return value;
}

void StoredSettings::loadFromDisk() {
  if (!SPIFFS.exists(dbPath)) {
    debugE("No container.db defined on load.");
    return;
  }

  File containerDb = SPIFFS.open(dbPath);

  size_t dbSize = containerDb.size();

  auto dbBuffer = std::make_unique<unsigned char[]>(dbSize);
  containerDb.read(dbBuffer.get(), dbSize);
  containerDb.close();
  auto *parsedJson = cJSON_Parse(reinterpret_cast<const char *>(dbBuffer.get()));
  if (parsedJson == nullptr) {
    debugE("Failed to parse settings json: %s", cJSON_GetErrorPtr());
  }
  debugE("Read settings json: %s", cJSON_Print(parsedJson));
  if (json != nullptr) {
    //cJSON_Delete(json);
  }
  json = parsedJson;
}

void StoredSettings::writeToDisk() {
  char *jsonString = cJSON_Print(json);
  updateDbFile(jsonString, strlen(jsonString) + 1);
  debugV("Container definition size written to disk: %s", jsonString);
}

void StoredSettings::updateDbFile(const char *buffer, size_t length) {
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
StoredSettings::StoredSettings() {
  json = cJSON_CreateObject();
}
void StoredSettings::setup() {
  loadFromDisk();
}
void StoredSettings::reset() {
  json = cJSON_CreateObject();
  writeToDisk();
}
void StoredSettings::runTransaction(std::function<void()> transaction) {
  commitOnSet = false;
  transaction();
  commitOnSet = true;
  writeToDisk();
}
