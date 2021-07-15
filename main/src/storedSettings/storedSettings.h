#pragma once

#include "fmt/printf.h"
#include <cJSON.h>
#include <optional>
#include <functional>
#include "wsdebug.h"
class StoredSettings {
 private:
  cJSON *json;

  void updateDbFile(const char *buffer, size_t length);
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

 public:
  StoredSettings();
  void setup();
  void reset();
  void setInt(const char *key, int value);
  void setString(const char *key, const char *value);
  void setBool(const char *key, bool value);

  std::optional<char *> getString(char *key);
  std::optional<int> getInt(char *key);
  std::optional<bool> getBool(char *key);

  void runTransaction(std::function<void()> transaction);

  FMT_VARIADIC(void, setIntF, int, fmt::CStringRef)
  inline void setIntF(int value, fmt::CStringRef format, fmt::ArgList args) {
    setInt(fmt::sprintf(format, args).c_str(), value);
  };

  FMT_VARIADIC(void, setStringF, const char*, fmt::CStringRef)
  inline void setStringF(const char *value, fmt::CStringRef format, fmt::ArgList args) {
    setString(fmt::sprintf(format, args).c_str(), value);
  };

  FMT_VARIADIC(void, setBoolF, bool, fmt::CStringRef)
  inline void setBoolF(bool value, fmt::CStringRef format, fmt::ArgList args) {
    setBool(fmt::sprintf(format, args).c_str(), value);
  };

  FMT_VARIADIC(std::optional<int>, getIntF, fmt::CStringRef)
  inline std::optional<int> getIntF(fmt::CStringRef format, fmt::ArgList args) {
    return getInt(const_cast<char *>(fmt::sprintf(format, args).c_str()));
  };

  FMT_VARIADIC(std::optional<bool>, getBoolF, fmt::CStringRef)
  inline std::optional<bool> getBoolF(const fmt::CStringRef format, fmt::ArgList args) {
    return getBool(const_cast<char *>(fmt::sprintf(format, args).c_str()));
  };

  FMT_VARIADIC(std::optional<char *>, getStringF, fmt::CStringRef)
  inline std::optional<char *> getStringF(fmt::CStringRef format, fmt::ArgList args) {
    return getString(const_cast<char *>(fmt::sprintf(format, args).c_str()));
  };
};

extern StoredSettings storedSettings;