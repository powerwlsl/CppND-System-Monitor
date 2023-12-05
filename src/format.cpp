#include "format.h"

#include <string>

using std::string;

// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string Format::ElapsedTime(long seconds) {
  int hours = seconds / 3600;
  seconds -= hours * 3600;

  int minutes = seconds / 60;
  seconds -= minutes * 60;

  // Create a C-string, print elapsed time in output format to it and then
  // convert it to a std::string
  char c[8];
  sprintf(c, "%02d:%02d:%02d", hours, minutes, seconds);
  string s(c);

  return s;
}