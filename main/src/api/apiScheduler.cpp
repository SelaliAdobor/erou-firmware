#include <dispense.h>
#include "api.h"
#include "dispenseSerialization.h"
void Api::addDispense(em::Request &req, em::Response &res) {

  auto[error, dispense] = req.parseJson<Dispense>();
  if (error) {
    return res.sendText(em::BadRequest, error.c_str());
  }
  const std::optional<double> &secondsUntil = dispense.secondsUntil();
  if (!secondsUntil.has_value()) {
    return res.sendText(em::BadRequest, "Unable to parse cron schedule");
  }
  dispenserManager->addDispense(dispense);

  res.sendJson(em::Ok, [&dispense = dispense, &secondsUntil](JsonDocument &reply) {
    reply["created"] = true;
    reply["dispense"] = dispense;
    reply["secondsUntilNext"] = *secondsUntil;
  });
}


