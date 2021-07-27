#pragma once
#include <functional>
#include "string_view"
namespace em {
struct WsMessage {
  std::string_view text;
  std::function<void(std::string_view)> reply;
};
}