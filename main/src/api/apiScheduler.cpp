#include <dispense.h>
#include "api.h"
#include "dispenseSerialization.h"
void Api::addDispense(em::Request &req, em::Response &res) {

  auto[error, dispense] = req.parseJson<Dispense>();
  if (error) {
    return res.sendText(em::BadRequest, error.c_str());
  }
  dispenserManager->addDispense(dispense);

  res.sendJson(em::Ok, [&dispense = dispense](JsonDocument &reply) {
    reply["created"] = true;
    reply["dispense"] = dispense;
  });
}


