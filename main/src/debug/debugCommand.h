#pragma once


struct DebugCommand {
  const char* name;
  void (*run)(const ShortString&);
};