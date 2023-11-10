#include <string>
#include <sstream>
#include <iostream>

#include "format.h"

using std::string;

// Adds a "0" for nice alignment
string Format::AddZeroIfNeeded(long number) {
    return number < 10 ? "0" + std::to_string(number) : std::to_string(number);
}

// Formats a given time in seconds into HH:MM:SS
string Format::ElapsedTime(long seconds) { 
    long hours, minutes;
    hours = seconds / 3600;
    seconds %= 3600;
    minutes = seconds / 60;
    seconds %= 60;

    std::ostringstream stream;
    stream << AddZeroIfNeeded(hours) << ":" << AddZeroIfNeeded(minutes) << ":" << AddZeroIfNeeded(seconds);
    return stream.str();     
}