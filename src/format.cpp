#include <string>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) { 
    int hour, minute;
    string result;
    hour = seconds / 3600;
    seconds = seconds % 3600;
    minute = seconds / 60;
    seconds = seconds % 60;
    result = std::to_string(hour) + ":" + std::to_string(minute) + ":" + std::to_string(seconds);
    
    
    return result; 
    }