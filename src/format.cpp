#include <string>

#include "format.h"

using std::string;

string Format::ElapsedTime(long seconds) {
  string hh_string;
  string mm_string;
  string ss_string;
  int HH = std::floor(seconds / SECONDS_IN_HOUR);
  seconds = seconds % SECONDS_IN_HOUR;
  int MM = std::floor(seconds / SECONDS_IN_MINUTE);
  seconds = seconds % SECONDS_IN_MINUTE;
  int SS = std::round(seconds);
  if (HH < 10) {
    hh_string = "0" + std::to_string(HH);
  } else {
    hh_string = std::to_string(HH);
  }
  if (MM < 10) {
    mm_string = "0" + std::to_string(MM);
  }
  else {
    mm_string = std::to_string(MM);
  }
  if (SS < 10) {
    ss_string = "0" + std::to_string(SS);
  } else {
    ss_string = std::to_string(SS);
  }
  string output_string = hh_string + ":" + mm_string + ":" + ss_string;
  return output_string;
}