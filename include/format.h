#ifndef FORMAT_H
#define FORMAT_H

#include <string>

namespace Format {
    std::string ElapsedTime(long); 
    std::string AddZeroIfNeeded(long);
}; // namespace Format

#endif