#include <config_constants.h>
#include <container.h>
#include "mongoose.h"
#include "api.h"
#include "requestUtil.h"

void Api::addContainer(HttpRequest &req, HttpReply &res) {
  int index;
  Container content;
  static DynamicJsonDocument json(2048);
  json.clear();
  auto error = deserializeJson(json, req.body.ptr, req.body.len);
  if (error) {
    res.sendText(BadRequest, error.c_str());
  }
  auto root = json.as<JsonObject>();
  auto missingFields = getJsonParams(&root,
                                     IntParam("index", &index),
                                     ShortStringParam ("id", &content.id),
                                     LongStringParam ("name", &content.name),
                                     LongStringParam("description", &content.description),
                                     IntParam("quantity", &content.quantity));

  if (!missingFields.empty()) {
    res.sendJson(BadRequest, [&](auto &reply) {
      auto replyFields = reply.createNestedArray("missingFields");
      for (const auto *field : missingFields) {
        debugV("Missing field %s", field);
        replyFields.add(field);
      }
    });
    return;
  }

  if (index > config::physical::containerCount) {
    return res.sendText(BadRequest, "exceeded container count");
  }

  containerManager->setContainerContent(index, content);

  res.sendJson(Ok, [&](auto &reply) {
    reply["created"] = true;
    reply["id"] = content.id.data();
    reply["name"] = content.name.data();
    reply["description"] = content.description.data();
    reply["quantity"] = content.quantity;
  });
}

void Api::getContainer(HttpRequest &req, HttpReply &res) {
  if (req.params.count("id") < 1) {
    debugE("Failed to parse index %s", req.url);
    return res.sendText(BadRequest, "failed to parse container number from path");
  }

  int containerIndex = atoi(req.params["id"].data());

  if (containerIndex > config::physical::containerCount) {
    return res.sendText(BadRequest, "exceeded countainer count");
  }

  auto container = containerManager->getContainerContent(containerIndex);

  if (!container.has_value()) {
    res.sendJson(Ok, [&](auto &reply) { reply["foundContainer"] = false; });
    return;
  }

  res.sendJson(Ok, [&](auto &reply) {
    reply["foundContainer"] = true;
    reply["id"] = container->id.data();
    reply["name"] = container->name.data();
    reply["description"] = container->description.data();
    reply["quantity"] = container->quantity;
  });
}



