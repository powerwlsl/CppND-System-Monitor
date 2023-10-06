#include "processor.h"

#include <unistd.h>

#include <sstream>

#include "linux_parser.h"

using std::string;
using std::vector;

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { return std::stof(LinuxParser::CpuUtilization()[0]); }
vector<long> Processor::getCurrUtilization() {
  vector<string> util = LinuxParser::CpuUtilization();
  long userCurr, niceCurr, systemCurr, idleCurr, iowaitCurr, irqCurr,
      softirqCurr, stealCurr, total, Idle, NonIdle;

  userCurr = std::stol(util[0]);
  niceCurr = std::stol(util[1]);
  systemCurr = std::stol(util[2]);
  idleCurr = std::stol(util[3]);