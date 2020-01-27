#include <string>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
std::string Format::Pad(string s, char c) {
  s.insert(s.begin(), 2 - s.size(), c);
  return s;
}

string Format::ElapsedTime(long seconds) {
  long lseconds = int(seconds % 60);
  long minutes = seconds / 60;
  long hours = minutes / 60;
  minutes = int(minutes % 60);
  std::string(Format::Pad(to_string(hours), '0') + ":" +
                     Format::Pad(to_string(minutes), '0') + ":" +
                     Format::Pad(to_string(lseconds), '0'));
}