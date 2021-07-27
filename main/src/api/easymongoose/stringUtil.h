#pragma once

namespace em{
size_t inline findNthOccurrence(std::string_view str,
                               char ch, int N) {
  int occur = 0;

  for (int i = 0; i < str.length(); i++) {
    if (str[i] == ch) {
      occur += 1;
    }
    if (occur == N)
      return i;
  }
  return str.length();
}
}