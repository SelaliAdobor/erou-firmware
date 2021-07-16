#include "dispense.h"
#include "croncpp.h"
#define FIELD_NAME(x) #x

void Dispense::serializeInto(StoredSettings *store, const char *rootKey) const {
  store->setStringF(id.c_str(), "%s" FIELD_NAME(id), rootKey);
  store->setStringF(name.c_str(), "%s" FIELD_NAME(name), rootKey);
  store->setStringF(cronSchedule.c_str(), "%s" FIELD_NAME(cronSchedule), rootKey);
  store->setIntF(static_cast<int>(containerIds.size()), "%s" "containerCount", rootKey);
  for (int i = 0; i < containerIds.size(); i++) {
    store->setStringF(containerIds[i].c_str(), "%s" "containerId-%d", rootKey, i);
  }
}

void Dispense::deserializeFrom(StoredSettings *store, const char *rootKey) {
  if (auto storedId = store->getStringF("%s" FIELD_NAME(id), rootKey)) {
    id = std::string(*storedId);
    debugI("id %s", id.c_str());
  }
  if (auto storedName = store->getStringF("%s" FIELD_NAME(name), rootKey)) {
    name = std::string(*storedName);
    debugI("name %s", name.c_str());
  }
  if (auto storedCron = store->getStringF("%s" FIELD_NAME(cronSchedule), rootKey)) {
    cronSchedule = std::string(*storedCron);
    debugI("cron %s", name.c_str());
  }
  if (auto countainerCount = store->getIntF("%s" "containerCount", rootKey)) {
    containerIds = ContainerIdList(*countainerCount);
    for (int i = 0; i < *countainerCount; i++) {
      if (auto containerId = store->getStringF("%s" "containerId-%d", rootKey, i)) {
        containerIds[i] = std::string(*containerId);
        debugI("container ID %s", name.c_str());
      }
    }
  }
}

#undef FIELD_NAME

double Dispense::secondsUntil(tm startingTime) {
  auto cron = cron::make_cron(cronSchedule);
  tm next = cron::cron_next(cron.value(), startingTime);
  time_t nowT = mktime(&startingTime);
  time_t nextT = mktime(&next);
  return difftime(nowT, nextT);
}
