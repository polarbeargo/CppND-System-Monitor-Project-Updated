#include <string>

#include "format.h"

using std::string;
using std::to_string;

std::string Format::Pad(string time) {
  if (time < 10) {
    display_time = "0" + std::to_string(time);
  } else {
    display_time = std::to_string(time);
  }
  return display_time;
}
// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) {
  long lseconds = int(seconds % 60);
  long minutes = seconds / 60;
  long hours = minutes / 60;
  minutes = int(minutes % 60);
  return std::string(Format::Pad(to_string(hours)) + ":" +
                     Format::Pad(to_string(minutes)) + ":" +
                     Format::Pad(to_string(seconds)));
}