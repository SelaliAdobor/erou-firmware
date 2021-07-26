#pragma once
#include "dispense.h"
#include "ArduinoJson.h"
#define FIELD_NAME(x) #x
namespace ARDUINOJSON_NAMESPACE {
template<>
struct Converter<Dispense> {
  static bool toJson(const Dispense &src, VariantRef dst) {
    dst[FIELD_NAME(id)] = std::string(src.id.c_str());
    dst[FIELD_NAME(name)] = std::string(src.name.c_str());
    dst[FIELD_NAME(cronSchedule)] = std::string(src.cronSchedule.c_str());
    auto jsonIds = dst.createNestedArray(FIELD_NAME(containerIds));
    for(const auto& id : src.containerIds){
      jsonIds.add(std::string(id.c_str()));
    }
    return true;
  }

  static Dispense fromJson(VariantConstRef src) {
    JsonArrayConst containers = src[FIELD_NAME(containerIds)];
    ContainerIdList ids;
    if (!containers.isNull()) {
      for (auto container:containers) {
        ids.push_back(ShortString(container.as<const char *>()));
      }
    }

    return Dispense{
        .id = ShortString(src[FIELD_NAME(id)].as<const char*>()),
        .name = ShortString(src[FIELD_NAME(name)].as<const char*>()),
        .cronSchedule = ShortString(src[FIELD_NAME(cronSchedule)].as<const char*>()),
        .containerIds = ids
    };
  }

  static bool checkJson(VariantConstRef src) {
    return src[FIELD_NAME(id)]
        && src[FIELD_NAME(name)];
  }
};
}
#undef FIELD_NAME