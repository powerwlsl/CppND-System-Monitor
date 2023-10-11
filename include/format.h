#ifndef FORMAT_H
#define FORMAT_H

#define SECONDS_IN_HOUR 3600
#define SECONDS_IN_MINUTE 60
#include <cmath>
#include <string>

namespace Format {
std::string ElapsedTime(long times);  // TODO: See src/format.cpp
};                                    // namespace Format

#endif