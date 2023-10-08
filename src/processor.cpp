#include "processor.h"

#include "linux_parser.h"

float Processor::Utilization() {
  long total = LinuxParser::Jiffies();
  long active = LinuxParser::ActiveJiffies();

  return (total != 0) ? static_cast<float>(active) / total : 0.0f;
}