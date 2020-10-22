#include "format.h"

#include <iostream>
#include <string>
#include <vector>

using std::string;

// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string Format::ElapsedTime(long seconds) {
  std::string hour = std::to_string(seconds / 3600);
  std::string minute = std::to_string((seconds % 3600) / 60);
  std::string second = std::to_string(seconds % 60);
  std::vector<std::string> v = {hour, minute, second};

  // prepend 0 in case the string length is 1
  for (auto i = 0; i < 3; ++i) {
    if (v[i].length() == 1) {
      v[i] = "0" + v[i];
    }
  }

  return v[0] + ":" + v[1] + ":" + v[2];
}