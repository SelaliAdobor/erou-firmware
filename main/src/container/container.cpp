#include "container.h"

#define FIELD_NAME(x) #x

void Container::serializeInto(json11::Json::object &dest) const {
  dest.insert({FIELD_NAME(name), name});
  dest.insert({FIELD_NAME(description), description});
  dest.insert({FIELD_NAME(quantity), quantity});
  dest.insert({FIELD_NAME(cron), cron});
}

void Container::deserializeFrom(json11::Json::object object) {
  name = object.at(FIELD_NAME(name)).string_value();
  description = object.at(FIELD_NAME(description)).string_value();
  cron = object.at(FIELD_NAME(cron)).string_value();
  quantity = object.at(FIELD_NAME(quantity)).int_value();
}
#undef FIELD_NAME
