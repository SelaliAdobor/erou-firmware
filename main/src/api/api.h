#pragma once
#include <containerManager.h>
#include "easymongoose.h"

class Api {
 private:
  void addContainer(em::Request &req, em::Response &res);
  void getContainer(em::Request &req, em::Response &res);
  void addDispense(em::Request &req, em::Response &res);
  ContainerManager *containerManager;
 public:
  explicit Api(ContainerManager *container_manager) : containerManager(container_manager) {}
  void setup();
};

