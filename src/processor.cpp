#include <vector>
#include <string>

#include "processor.h"

float Processor::Utilization() {
  vector<string> jiffies = LinuxParser::CpuUtilization();
  float Idle = std::stof(jiffies[CPUStates::kIdle_]) + std::stof(jiffies[CPUStates::kIOwait_]);
  float NonIdle = std::stof(jiffies[CPUStates::kUser_]) + std::stof(jiffies[CPUStates::kNice_]) + std::stof(jiffies[CPUStates::kSystem_]) + std::stof(jiffies[CPUStates::kIRQ_]) + std::stof(jiffies[CPUStates::kSoftIRQ_]) + std::stof(jiffies[CPUStates::kSteal_]);
  float Total = Idle + NonIdle;
  float totald = Total - prevTotal;
  float idled = Idle - prevIdle;
  prevIdle = Idle;
  prevNonIdle = NonIdle;
  prevTotal = Total;
  return (totald - idled) / totald;
}