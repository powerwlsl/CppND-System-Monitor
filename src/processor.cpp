#include <thread>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <sstream> 

#include "processor.h"
#include "linux_parser.h"

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 
  std::vector<long> start = ParseCPUData();
  std::this_thread::sleep_for(std::chrono::seconds(1));
  std::vector<long> end = ParseCPUData();

  if(start.empty() || end.empty()) {
    return 0.0f;
  }

  long startIdle = start[LinuxParser::CPUStates::kIdle_] + start[LinuxParser::CPUStates::kIOwait_];
  long endIdle = end[LinuxParser::CPUStates::kIdle_] + end[LinuxParser::CPUStates::kIOwait_];
  long startTotal = 0;
  long endTotal = 0;

  for (long value : start) {
    startTotal += value;
  }

  for (long value : end) {
    endTotal += value;
  }

  float idleDifference = endIdle - startIdle;
  float totalDifference = endTotal - startTotal;
  return (totalDifference - idleDifference) / totalDifference;
 }


std::vector<long> Processor::ParseCPUData() {
  std::ifstream filestream(LinuxParser::kProcDirectory + LinuxParser::kStatFilename);
  std::string line, cpu;
  std::vector<long> values;

  if (filestream.is_open() && std::getline(filestream, line)) {
    std::istringstream linestream(line);
    linestream >> cpu;
    long value;
    while (linestream >> value) {
      values.push_back(value);
    }
  }
  return values;  
}