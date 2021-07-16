#pragma once

#include <containerManager.h>
#include <motion.h>
#include "dispense.h"
class DispenseManager {
 private:
  static constexpr size_t maxDispenseDefinitions = 5 * 7; //5 regiments per day
  using DispenseList = etl::vector<Dispense, maxDispenseDefinitions>;
  DispenseList dispenses;
  ContainerManager *containerManager;
  StoredSettings *storedSettings;
  Motion *motion;
  TickType_t lastDispenseTaskRun;
  [[noreturn]] void runDispenseTask();
 public:
  explicit DispenseManager(ContainerManager *container_manager, StoredSettings *stored_settings) :
      containerManager(container_manager), storedSettings(stored_settings) {}
  void setup();

  void loadFromDisk();
  void writeToDisk();

  double secondsUntilNextDispense();
  std::optional<Dispense> getNextDispense(tm startingFrom);
  std::optional<Dispense> getNextDispense();

  void runDispense(const Dispense &dispense);
};