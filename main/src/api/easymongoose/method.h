#pragma once

namespace em::method {
enum class Value {
  Get, Post, Put
};

static constexpr Value Get = Value::Get;
static constexpr Value Post = Value::Post;
static constexpr Value Put = Value::Put;

inline const char *toString(Value method) {
  switch (method) {
    case Value::Get: return "GET";
    case Value::Post: return "POST";
    case Value::Put: return "PUT";
    default: return "undefined";
  }
}
}