#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>

#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;
/*You need to complete the mentioned TODOs in order to satisfy the rubric criteria "The student will be able to extract and display basic data about the system."

You need to properly format the uptime. Refer to the comments mentioned in format. cpp for formatting the uptime.*/

// TODO: Return the system's CPU
Processor& System::Cpu() { return cpu_; }

// TODO: Return a container composed of the system's processes
vector<Process>& System::Processes() { 
  std::vector<int> pids = LinuxParser::Pids();
  processes_.clear();
  for (int pid : pids) {
    Process process(pid);
    processes_.push_back(process);
  }
  std::sort(processes_.begin(), processes_.end());
  return processes_;
 }

// TODO: Return the system's kernel identifier (string)
std::string System::Kernel() { 
  std::ifstream filestream(LinuxParser::kProcDirectory + LinuxParser::kVersionFilename);
  std::string line;
  
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    std::string ignore;

    for (int i = 0; i < 2; i++) {
      linestream >> ignore;
    }
    linestream >> line;
  }
  return line;
}

// TODO: Return the system's memory utilization
float System::MemoryUtilization() { 
  std::ifstream filestream(LinuxParser::kProcDirectory + LinuxParser::kMeminfoFilename);
  long memTotal, memFree, buffers, cached;

  if (filestream.is_open()) {
    std::string line, key;
    while (std::getline(filestream,  line)) {
      std::istringstream linestream(line);

      linestream >> key;
      if (key == "MemTotal:") {
        linestream >> memTotal;
      } else if (key == "MemFree:") {
        linestream >> memFree;
      } else if (key == "Buffers:") {
        linestream >> buffers;
      } else if (key == "Cached:") {
        linestream >> cached;
      }
    }    
  }
  float utilizedMemory = memTotal - memFree - buffers - cached;
  return utilizedMemory / memTotal;
 }

// TODO: Return the operating system name
std::string System::OperatingSystem() { 
  std::ifstream filestream(LinuxParser::kOSPath);
  std::string line, key, value;

  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');

      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }    
  }
  return value;
 }

int GetValueFromProcStat(const std::string& key) {
  std::ifstream filestream(LinuxParser::kProcDirectory + LinuxParser::kStatFilename);
  std::string line, current_key;
  int value;
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);

      linestream >> current_key;
      if (current_key == key) {
        linestream >> value;
        return value;
      }
    }    
  }
  return -1;
}
// TODO: Return the number of processes actively running on the system
int System::RunningProcesses() { 
  return GetValueFromProcStat("procs_running");
 }

// TODO: Return the total number of processes on the system
int System::TotalProcesses() {
  return GetValueFromProcStat("processes");
 }

// TODO: Return the number of seconds since the system started running
long int System::UpTime() { 
  std::ifstream filestream(LinuxParser::kProcDirectory + LinuxParser::kUptimeFilename);
  if (filestream.is_open()) {
    double uptime_second;
    filestream >> uptime_second;
    return static_cast<long int>(uptime_second);
  }
  return 0;
}
