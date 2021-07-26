#pragma once

#define FIELD_NAME(x) #x
namespace ARDUINOJSON_NAMESPACE {
template<>
struct Converter<Container> {
  static bool toJson(const Container &src, VariantRef dst) {
    dst[FIELD_NAME(id)] = src.id.c_str();
    dst[FIELD_NAME(name)] = src.name.c_str();
    dst[FIELD_NAME(description)] = src.description.c_str();
    dst[FIELD_NAME(quantity)] = src.quantity;
    return true;
  }

  static Container fromJson(VariantConstRef src) {
    return Container{
        .id = ShortString(src[FIELD_NAME(id)].as<const char*>()),
        .name = LongString (src[FIELD_NAME(name)].as<const char*>()),
        .description = LongString(src[FIELD_NAME(description)].as<const char*>()),
        .quantity = src[FIELD_NAME(quantity)]
    };
  }

  static bool checkJson(VariantConstRef src) {
    return src[FIELD_NAME(id)]
        && src[FIELD_NAME(name)]
        && src[FIELD_NAME(description)]
        && src[FIELD_NAME(quantity)];
  }
};
}
#undef FIELD_NAME
