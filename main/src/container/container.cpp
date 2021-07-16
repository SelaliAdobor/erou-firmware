#include <wsdebug.h>
#include "container.h"

#define FIELD_NAME(x) #x
void Container::deserializeFrom(StoredSettings* store, const char *rootKey) {
  if (auto storedId = store->getStringF("%s" FIELD_NAME(id), rootKey)) {
    id = std::string(storedId.value());
    debugI("id %s", id.c_str());
  }
  if (auto storedName = store->getStringF("%s" FIELD_NAME(name), rootKey)) {
    name = std::string(storedName.value());
    debugI("name %s", name.c_str());
  }
  if (auto storedDescription = store->getStringF("%s" FIELD_NAME(description), rootKey)) {
    description = *(new std::string(storedDescription.value()));
   debugI("description %s", description.c_str());
  }
  if (auto storedQuantity = store->getIntF("%s" FIELD_NAME(quantity), rootKey)) {
    quantity = int(storedQuantity.value());
  }
}

void Container::serializeInto(StoredSettings* store, const char *rootKey) const {
  store->setStringF(id.c_str(), "%s" FIELD_NAME(id), rootKey);
  store->setStringF(name.c_str(), "%s" FIELD_NAME(name), rootKey);
  store->setStringF(description.c_str(), "%s" FIELD_NAME(description), rootKey);
  store->setIntF(quantity, "%s" FIELD_NAME(quantity), rootKey);
}

#undef FIELD_NAME
