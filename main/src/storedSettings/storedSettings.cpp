#include <wsdebug.h>
#include <SPIFFS.h>
#include <cjson_util.h>
#include "storedSettings.h"

StoredSettings storedSettings = StoredSettings();


void StoredSettings::loadFromDisk() {
  if (!SPIFFS.exists(dbPath)) {
    debugE(logtags::storage, "No container.db defined on load.");
    return;
  }

  File containerDb = SPIFFS.open(dbPath);

  size_t dbSize = containerDb.size();
  auto dbBuffer = std::make_unique<unsigned char[]>(dbSize);
  containerDb.read(dbBuffer.get(), dbSize);
  containerDb.close();
  DeserializationError error = deserializeJson(backingDocument, reinterpret_cast<const char *>(dbBuffer.get()));
  if (error) {
    debugE(logtags::storage, "Failed to parse settings json: %s", error.c_str());
    backingDocument.clear();
  }
  if(backingDocument.isNull()){
    backingDocument["settings"] = true;
  }
  debugE(logtags::storage, "Read settings json size: %d", dbSize);
}

void StoredSettings::writeToDisk() {
  static const char *writeMode = "w+";

  File file = SPIFFS.open(dbTempPath, writeMode);
  size_t bytesWritten = serializeJson(backingDocument, file);
  file.flush();
  debugV(logtags::storage, "Container definition size written to disk: %zu", bytesWritten);
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
  SPIFFS.rename(dbTempPath, dbPath);
  SPIFFS.remove(dbBackupPath);
  SPIFFS.remove(dbTempPath);
}


void StoredSettings::setup() {
  loadFromDisk();
}

void StoredSettings::reset() {
  for(int i = 0; i < backingDocument.size(); i++){
    backingDocument.remove(i);
  }
  backingDocument["settings"] = true;
  SPIFFS.remove(dbPath);
  writeToDisk();
}

void StoredSettings::runTransaction(const std::function<void(JsonDocument&)>& transaction) {
  commitOnSet = false;
  transaction(backingDocument);
  commitOnSet = true;
  writeToDisk();
}
