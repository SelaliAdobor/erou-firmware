//
// Created by Selali Adobor on 7/9/21.
//

#ifndef HELLO_WORLD_MAIN_SRC_SCHEDULER_SCHEDULER_H_
#define HELLO_WORLD_MAIN_SRC_SCHEDULER_SCHEDULER_H_

#include <containerManager.h>
class Scheduler {
  ContainerManager* containerManager;
  static constexpr const char* ntpServer = "pool.ntp.org";
 public:
  explicit Scheduler( ContainerManager* container_manager);
 private:
  double secondsUntilNextDispense();
};

#endif //HELLO_WORLD_MAIN_SRC_SCHEDULER_SCHEDULER_H_
