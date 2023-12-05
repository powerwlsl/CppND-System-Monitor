#include "processor.h"

#include "linux_parser.h"

// Return the aggregate CPU utilization
float Processor::Utilization() {
  activejiffies_ = LinuxParser::ActiveJiffies();
  jiffies_ = LinuxParser::Jiffies();
  utilization_ = std::abs((float)(activejiffies_ - prevactivejiffies_) /
                          (float)(jiffies_ - prevjiffies_));
  prevactivejiffies_ = activejiffies_;
  prevjiffies_ = jiffies_;

  return utilization_;
}