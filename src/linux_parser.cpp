#include <unistd.h>
#include <string>
#include <vector>
#include <filesystem>

#include "linux_parser.h"

std::string LinuxParser::OperatingSystem() {
  std::string line;
  std::string key;
  std::string value;
  std::ifstream filestream(kOSPath);
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

std::string LinuxParser::Kernel() {
  std::string os, version, kernel;
  std::string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// Updated using std::filesystem. Parts of the code from https://en.cppreference.com/w/cpp/filesystem
std::vector<int> LinuxParser::Pids() {
  std::vector<int> pids;
  for (const auto& entry : std::filesystem::directory_iterator(kProcDirectory)) {
    if (entry.is_directory()) {
      // Is every character of the name a digit?
      std::string filename{entry.path().stem().string()};
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  return pids;
}

float LinuxParser::MemoryUtilization() { 
  std::string line;
  std::string key;
  long value, mem_total, mem_free;
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "MemTotal:") {
          mem_total = value;
        } else if (key == "MemFree:") {
          mem_free = value;
        }
      }
    }
    return (float)(mem_total - mem_free) / mem_total;
  }
  return 0.0f;
}

// Uptime in seconds
long LinuxParser::UpTime() { 
  long uptime;
  std::string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> uptime;
  }
  return uptime;
}

// Uses the CPUStates enum values and some bitwise logic to filter the jiffies to extract
long LinuxParser::SystemJiffiesFiltered(const int filter) {
  auto jiffies_raw = CpuUtilization();
  long jiffies {0};
  for (LinuxParser::CPUStates state : kAllStates) {
    if (state & filter)
      jiffies += jiffies_raw[state];
  }
  return jiffies;
}

long LinuxParser::Jiffies() {
  return SystemJiffiesFiltered(kAllJiffies);
}

long LinuxParser::ActiveJiffies(int pid) { 
  std::string line, foo;
  long jiffies;
  long all_jiffies {0};
  std::ifstream filestream(kProcDirectory + "/" + std::to_string(pid) + kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    for (int i=0; i!=17; i++) {
      if (i >= 13) {
        linestream >> jiffies;
        all_jiffies += jiffies;
      } else {
        linestream >> foo; // Discarding uninteresting entries
      }
    }
    return all_jiffies;
  }
  return 0L; 
}

long LinuxParser::ActiveJiffies() { 
  return SystemJiffiesFiltered(kAllActive);
}

long LinuxParser::IdleJiffies() {
  return SystemJiffiesFiltered(kAllIdle);
}

// Extracts each utilization type in a map with CPUStates as keys for further treatment
std::map<LinuxParser::CPUStates, long> LinuxParser::CpuUtilization() { 
  std::string line, key;
  std::map<LinuxParser::CPUStates, long> utilizations;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> key;
    for (CPUStates state : kAllStates) {
        linestream >> utilizations[state];
    }
  }
  return utilizations;
}

int LinuxParser::TotalProcesses() { 
  std::string line;
  std::string key;
  long value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "processes") {
          return value;
        }
      }
    }
  }
  return 0; 
}

int LinuxParser::RunningProcesses() {
  std::string line;
  std::string key;
  long value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "procs_running") {
          return value;
        }
      }
    }
  }
  return 0; 
}

std::string LinuxParser::Command(int pid) { 
  std::string cmd;
  std::ifstream filestream(kProcDirectory + "/" + std::to_string(pid) + kCmdlineFilename);
  if (filestream.is_open()) {
    std::getline(filestream, cmd);
    return cmd;
  }
  return std::string(); 
}

std::string LinuxParser::Ram(int pid) {
  std::string line, key;
  long value;
  std::ifstream filestream(kProcDirectory + "/" + std::to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "VmSize:") {
          return std::to_string(value / 1000); // Conversion to MB
        } 
      }
    }
  }
  return std::string(); 
}

std::string LinuxParser::Uid(int pid) { 
  std::string line;
  std::string key, value;
  std::ifstream filestream(kProcDirectory + "/" + std::to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "Uid:") {
          return value;
        } 
      }
    }
  }
  return std::string(); 
}

std::string LinuxParser::User(int pid) { 
  std::string line;
  std::string uname, pwd, uid;
  std::string true_uid = Uid(pid);
  std::ifstream filestream(kPasswordPath);
  if (true_uid != std::string() && filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> uname >> pwd >> uid) {
        if (uid == true_uid) {
          return uname;
        } 
      }
    }
  }
  return std::string(); 
}

long LinuxParser::UpTime(int pid) {
  std::string line, foo;
  long starttime; // Starting time of the process in clock ticks from system boot
  std::ifstream filestream(kProcDirectory + "/" + std::to_string(pid) + kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    for (int i=0; i!=21; i++) {
      linestream >> foo; // Skipping through all uninteresting entries
    }
    linestream >> starttime; 
    starttime /= sysconf(_SC_CLK_TCK);
    long uptime = UpTime(); // Time since system boot
    return uptime - starttime; // Up time of the process
  }
  return 0L; 
}
