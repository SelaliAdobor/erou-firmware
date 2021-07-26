#pragma once

#include "fmt/printf.h"
#include <cJSON.h>
#include <optional>
#include <functional>
#include "wsdebug.h"
#include "cjson_util.h"
#include "ArduinoJson.h"
class StoredSettings {
 private:
  /**
   * Write process avoids losing data if power is lost
   * 1. Move main DB to backup path
   * 2. Move temporary DB to main path
   * 3. Delete backup
   */
  static constexpr const char *dbPath = "/settings.json";
  static constexpr const char *dbTempPath = "/settings.json.tmp";
  static constexpr const char *dbBackupPath = "/settings.json.tmp.bak";

  void loadFromDisk();
  void writeToDisk();
  bool commitOnSet = true;

  DynamicJsonDocument backingDocument = DynamicJsonDocument(2048);
 public:
  void setup();
  void reset();

  template <class T>
  T get(std::string_view name){
    return backingDocument[name].as<T>();
  }

  template <class T>
  void set(std::string_view name, T value){
     backingDocument[name] = value;
     if(commitOnSet){
       writeToDisk();
     }
  }

  void runTransaction(const std::function<void(JsonDocument&)>& run);
};

extern StoredSettings storedSettings;