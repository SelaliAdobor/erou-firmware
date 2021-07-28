#include <config_constants.h>
#include <container.h>
#include "api.h"
#include "requestUtil.h"
#include "httpStatusCodes.h"
#include "containerSerialization.h"

void Api::addContainer(em::Request &req, em::Response &res) {
  int index;
  Container content;
  auto[error, root] = req.parseJson<JsonObject>();

  if (error) {
    res.sendText(em::BadRequest, error.c_str());
    return;
  }

  auto missingFields = getJsonParams(&root,
                                     IntParam("index", &index),
                                     SerializableParam<Container>("container", &content)
  );

  if (!missingFields.empty()) {
    res.sendMissingFields(missingFields);
    return;
  }

  if (index > config::physical::containerCount) {
    return res.sendText(em::BadRequest, "exceeded container count");
  }

  bool creatingNew = !containerManager->getById(content.id.data()).has_value();
  containerManager->setContainerContent(index, content);

  res.sendJson(em::Ok, [&](auto &reply) {
    reply["created"] = creatingNew;
    reply["container"] = content;
  });
}

void Api::getContainer(em::Request &req, em::Response &res) {
  if (req.params.count("id") < 1) {
    debugE(logtags::network, "Failed to parse index %s", req.url.data());
    return res.sendText(em::BadRequest, "failed to parse container number from path");
  }

  int containerIndex = 3;// atoi(req.params["id"].data());

  if (containerIndex > config::physical::containerCount) {
    return res.sendText(em::BadRequest, "exceeded countainer count");
  }

  auto container = containerManager->getContainerContent(containerIndex);

  if (!container.has_value()) {
    res.sendJson(em::Ok, [&](auto &reply) { reply["foundContainer"] = false; });
    return;
  }

  res.sendJson(em::Ok, [&](auto &reply) {
    reply["foundContainer"] = true;
    reply["container"] = container.value();
  });
}



