#include <string>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) { 
  long hours = seconds / 3600;
  long minutes = (seconds % 3600) / 60;
  long secs = seconds % 60;

  string hoursStr = std::to_string(hours);
  string minutesStr = std::to_string(minutes);
  string secsStr = std::to_string(secs);

  if (hoursStr.length() == 1) {
    hoursStr = "0" + hoursStr;
  }

  if (minutesStr.length() == 1) {
    minutesStr = "0" + minutesStr;
  }

  if (secsStr.length() == 1) {
    secsStr = "0" + secsStr;
  }

  return hoursStr + ":" + minutesStr + ":" + secsStr;
 }