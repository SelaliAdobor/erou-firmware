#pragma once
#include "containerManager.h"
#include "ArduinoJson.h"
#include "container.h"
#include "config_constants.h"

class ContainerManager {
 private:
  Container containerContents[config::physical::containerCount];
  bool hasContent[config::physical::containerCount];
  static const size_t maxContainerDBSize = 4048;

  void updateDbFile(char *buffer, size_t length);
  static constexpr const char* jsonContainerKey ="container";
  static constexpr const char* jsonVersionKey = "version";

  /**
   * Write process avoids losing data if power is lost
   * 1. Move main DB to backup path
   * 2. Move temporary DB to main path
   * 3. Delete backup
   */
  static constexpr const char* dbPath = "/cfg/containers.json";
  static constexpr const char* dbTempPath = "/cfg/containers.json.tmp";
  static constexpr const char* dbBackupPath = "/cfg/containers.json.tmp.bak";

 public:
  ContainerManager();

  std::optional<Container> getContainerContent(int container);
  void setContainerContent(int container, Container content);

  void setup();

  void loadFromDisk();
  void writeToDisk();
};


