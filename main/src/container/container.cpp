#include "binn_util.h"
#include "container.h"

#define FIELD_NAME(x) #x
void Container::serializeInto(JsonObject &dest) const {
  dest.getOrAddMember(FIELD_NAME(name)).set(name);
  dest.getOrAddMember(FIELD_NAME(description)).set(description);
  dest.getOrAddMember(FIELD_NAME(quantity)).set(quantity);
  dest.getOrAddMember(FIELD_NAME(cron)).set(cron);
}

void Container::deserializeFrom(JsonObject object) {
  name = object.getMember(FIELD_NAME(name)).as<std::string>();
  description = object.getMember(FIELD_NAME(description)).as<std::string>();
  cron = object.getMember(FIELD_NAME(cron)).as<std::string>();
  quantity = object[FIELD_NAME(quantity)];
}
#undef FIELD_NAME
