#include <config_constants.h>
#include <container.h>
#include "api.h"
#include "requestUtil.h"

void Api::addContainer(AsyncWebServerRequest *request) {
  bool hasMissingField = sendErrorIfMissing(request, {
      "id", "name", "index", "description", "startingQuantity", "cron"
  });

  if (hasMissingField) {
    return;
  }

  int64_t index = requestParamAs<int64_t>(request, "index");

  if (index > config::physical::containerCount) {
   return replyWithError(request, "exceeded container count");
  }

  Container content = {
      .id =  requestParamAs<ShortString>(request, "id"),
      .name =  requestParamAs<ShortString>(request, "name"),
      .description = requestParamAs<ShortString>(request, "description"),
      .quantity = requestParamAs<int>(request, "quantity"),
  };

  if (content.id.is_truncated()) {
    return replyWithError(request, "id exceeded max length");
  }

  if (content.name.is_truncated()) {
    return replyWithError(request, "name exceeded max length");
  }
  containerManager->setContainerContent(static_cast<int>(index), content);

  auto *response = cJSON_CreateObject();
  cJSON_AddStringToObject(response, "id", content.id.c_str());
  cJSON_AddStringToObject(response, "name", content.name.c_str());
  cJSON_AddStringToObject(response, "description", content.description.c_str());
  cJSON_AddNumberToObject(response, "quantity", content.quantity);

  request->send(200, "text/json", safePrint(response).get());
}

void Api::getContainer(AsyncWebServerRequest *request) {
  int containerIndex = 0;

  if (sscanf(request->url().c_str(), "/containers/%d", &containerIndex) != 1) {
    debugE("Failed to parse index %s %d", request->url().c_str(), containerIndex);
    return replyWithError(request, "failed to parse container number from path");
  }

  if (containerIndex > config::physical::containerCount) {
    return replyWithError(request, "exceeded max container count");
  }

  auto container = containerManager->getContainerContent(containerIndex);
  auto *response = cJSON_CreateObject();
  if (!container.has_value()) {
    cJSON_AddBoolToObject(response, "foundContainer", 0);
    replyWithJson(request, 200, response);
    return;
  };

  cJSON_AddBoolToObject(response, "foundContainer", 1);
  cJSON_AddStringToObject(response, "id", container->id.c_str());
  cJSON_AddStringToObject(response, "name", container->name.c_str());
  cJSON_AddStringToObject(response, "description", container->description.c_str());
  cJSON_AddNumberToObject(response, "quantity", container->quantity);
  replyWithJson(request, 200, response);
}


