#pragma once

#include "etl_types.h"
#include "ArduinoJson.h"
class Container {
 public:
  ShortString id;
  LongString name;
  LongString description;
  int quantity;
#define FIELD_NAME(x) #x

   bool canConvertFromJson(JsonVariantConst src, const Container &ct) {

    return !src[FIELD_NAME(id)].isNull()
        && !src[FIELD_NAME(name)].isNull()
        && !src[FIELD_NAME(description)].isNull()
        && !src[FIELD_NAME(quantity)].isNull();
  }
#undef FIELD_NAME

};
