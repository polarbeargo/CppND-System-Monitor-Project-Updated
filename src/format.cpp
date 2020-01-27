#include <string>

#include "format.h"

using std::string;
using std::to_string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) {
  long lseconds = int(seconds % 60);
  long minutes = seconds / 60;
  long hours = minutes / 60;
  minutes = int(minutes % 60);
  string display_hours = "";
  if (hours < 10) {
    display_hours = "0" + std::to_string(hours);
  } else {
    display_hours = std::to_string(hours);
  }
  string result = display_hours + ":" + std::to_string(minutes) + ":" +
                  std::to_string(lseconds);
  return result;
}