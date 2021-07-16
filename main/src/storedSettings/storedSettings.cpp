#include <wsdebug.h>
#include <SPIFFS.h>
#include <cjson_util.h>
#include "storedSettings.h"

StoredSettings storedSettings = StoredSettings();

void StoredSettings::setInt(const char *key, int value) {
  auto *property = cJSON_GetObjectItem(json.get(), key);
  if (property != nullptr) {
    cJSON_ReplaceItemInObject(json.get(), key, cJSON_CreateNumber(value));
  } else {
    cJSON_AddNumberToObject(json.get(), key, static_cast<int>(value));
  }
  if (commitOnSet) {
    writeToDisk();
  }
}

void StoredSettings::setString(const char *key, const char *value) {
  auto *property = cJSON_GetObjectItem(json.get(), key);
  if (property != nullptr) {
    cJSON_ReplaceItemInObject(json.get(), key, cJSON_CreateString(value));
  } else {
    cJSON_AddStringToObject(json.get(), key, value);
  }
  if (commitOnSet) {
    writeToDisk();
  }
}

void StoredSettings::setBool(const char *key, bool value) {
  setInt(key, static_cast<int>(value));
}

std::optional<int> StoredSettings::getInt(const char *key) {
  auto *property = cJSON_GetObjectItem(json.get(), key);
  if (property == nullptr || cJSON_IsNull(property)) {
    return {};
  }
  double value = cJSON_GetNumberValue(property);
  return static_cast<int>(value);
}

std::optional<bool> StoredSettings::getBool(const char *key) {
  auto *property = cJSON_GetObjectItem(json.get(), key);
  if (property == nullptr || cJSON_IsNull(property)) {
    return {};
  }
  double value = cJSON_GetNumberValue(property);
  return static_cast<bool>(value);
}
std::optional<char *> StoredSettings::getString(const char *key) {
  auto *property = cJSON_GetObjectItem(json.get(), key);
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
  auto parsedJson = safeParse(reinterpret_cast<const char *>(dbBuffer.get()));
  if (parsedJson == nullptr) {
    debugE("Failed to parse settings json: %s", cJSON_GetErrorPtr());
  }
  debugE("Read settings json: %s", safePrint(parsedJson.get()).get());
  json = std::move(parsedJson);
}

void StoredSettings::writeToDisk() {
  auto jsonString = safePrint(json.get());
  updateDbFile(jsonString.get(), strlen(jsonString.get()) + 1);
  debugV("Container definition size written to disk: %s", jsonString.get());
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
  json = safeParse("{}");
}

void StoredSettings::setup() {
  loadFromDisk();
}

void StoredSettings::reset() {
  json = safeParse("{}");
  writeToDisk();
}

void StoredSettings::runTransaction(const std::function<void()>& transaction) {
  commitOnSet = false;
  transaction();
  commitOnSet = true;
  writeToDisk();
}
