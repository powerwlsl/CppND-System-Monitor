#include "processor.h"
#include "linux_parser.h"

Processor::Processor() {
    last_jiffies_ = LinuxParser::Jiffies();
    last_active_jiffies_ = LinuxParser::ActiveJiffies();
}

// Aggregate CPU utilization. Formula given in https://stackoverflow.com/questions/23367857/accurate-calculation-of-cpu-usage-given-in-percentage-in-linux
float Processor::Utilization() { 
    long jiffies = LinuxParser::Jiffies();
    long active_jiffies = LinuxParser::ActiveJiffies();
    float util = (float)(active_jiffies - last_active_jiffies_) / (jiffies - last_jiffies_);
    last_active_jiffies_ = active_jiffies;
    last_jiffies_ = jiffies;
    return util;
}