#pragma once

#include <containerManager.h>
#include <motion.h>
#include "dispense.h"
class DispenseManager {
 private:
  static constexpr size_t maxDispenseDefinitions = 5 * 7; //5 regiments per day

  std::vector<Dispense> dispenses;
  ContainerManager *containerManager;
  StoredSettings *storedSettings;
  Motion *motion;
  TickType_t lastDispenseTaskRun;
  [[noreturn]] void runDispenseTask();
  bool invalidateDispense;
 public:
  explicit DispenseManager(ContainerManager *container_manager, StoredSettings *stored_settings) :
      containerManager(container_manager), storedSettings(stored_settings) {}
  void setup();

  void loadFromDisk();
  void writeToDisk();

  std::optional<Dispense> getNextDispense(time_t startingFrom);
  std::optional<Dispense> getNextDispense();
  void addDispense(Dispense dispense);
  std::optional<Dispense> getDispenseById(const ShortString& id);
  void runDispense(const Dispense &dispense);
};