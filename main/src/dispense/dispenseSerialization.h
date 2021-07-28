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
    for (const auto &id : src.containerIds) {
      jsonIds.add(std::string(id));
    }
    return true;
  }

  static Dispense fromJson(VariantConstRef src) {
    auto root = src.as<JsonObjectConst>();

    auto dispense = Dispense{
        .id = ShortString(root[FIELD_NAME(id)].as<const char *>()),
        .name = ShortString(root[FIELD_NAME(name)].as<const char *>()),
        .cronSchedule = ShortString(root[FIELD_NAME(cronSchedule)].as<const char *>()),
        .containerIds = {}
    };

    JsonArrayConst containers = root[FIELD_NAME(containerIds)];
    if (!containers.isNull()) {
      for (auto container : containers) {
        dispense.containerIds.push_back(container.as<std::string>());
      }
    }
    return dispense;
  }

  static bool checkJson(VariantConstRef src) {
    return src[FIELD_NAME(id)].is<std::string>()
        && src[FIELD_NAME(name)].is<std::string>()
        && src[FIELD_NAME(cronSchedule)].is<std::string>()
        && src[FIELD_NAME(cronSchedule)].as<std::string>().size() > 4;
  }
};
}
#undef FIELD_NAME