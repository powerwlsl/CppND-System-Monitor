#include <string>
#include <sstream>
#include <iomanip>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long second) {
  int hours, minutes, seconds;
  std::stringstream output;
  hours = (second / 3600) % 100; // Number of hours returns to 0 after 99
  minutes = (second / 60) % 60;
  seconds = second % 60;
  output << std::setfill('0') << std::setw(2) << hours << ":"
         << std::setfill('0') << std::setw(2) << minutes << ":"
         << std::setfill('0') << std::setw(2) << seconds;
  return output.str();
}