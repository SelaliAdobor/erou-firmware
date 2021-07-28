#pragma once
#include <containerManager.h>
#include "easymongoose.h"
#include "dispenserManager.h"
class Api {
 private:
  void addContainer(em::Request &req, em::Response &res);
  void getContainer(em::Request &req, em::Response &res);
  void addDispense(em::Request &req, em::Response &res);
  ContainerManager *containerManager;
  DispenseManager *dispenserManager;
 public:
  explicit Api(ContainerManager *container_manager, DispenseManager *dispenser_manager)
      : containerManager(container_manager), dispenserManager(dispenser_manager) {}
  void setup();
};

