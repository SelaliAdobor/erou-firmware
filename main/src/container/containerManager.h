#pragma once
#include "containerManager.h"
#include "container.h"
#include "config_constants.h"
#include "etl/unordered_map.h"

static const char *const containerEmptyKey = "containers/contents/%s/isEmpty";
static const char *const containerRootKey = "containers/contents/%s/";
using ContainerMap = etl::unordered_map<int, Container, config::physical::containerCount>;

//TODO: Refactor to separate container definition from container contents
class ContainerManager {
 private:
  ContainerMap containerContents;
  static const size_t maxContainerDBSize = 8048;

  void updateDbFile(const char *buffer, size_t length);
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
  ContainerMap getAllLoadedContainers();
  void setup();

  void loadFromDisk();
  void writeToDisk();
};


