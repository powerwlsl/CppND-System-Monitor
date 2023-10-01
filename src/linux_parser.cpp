#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>

#include "linux_parser.h"
#include "system.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
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

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
  return System::MemoryUtilization();
 }

// TODO: Read and return the system uptime
long LinuxParser::UpTime() { 
  return System::UpTime();
 }

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() {
  std::ifstream filestream(kProcDirectory + kStatFilename);
  std::string line, cpu;

  long user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice;

  if (filestream.is_open() && std::getline(filestream, line)) {
    std::istringstream linestream(line);
    linestream >> cpu >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal >> guest >> guest_nice;
    return user + nice + system + idle + iowait + irq + softirq + steal + guest + guest_nice;
  }

  return 0;
  
 }

// TODO: Read and return the number of active jiffies for a PID
long LinuxParser::ActiveJiffies(int pid) { 
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatFilename);

  std::string line, value;
  long utime, stime, cutime, cstime;

  if (filestream.is_open() && std::getline(filestream, line)) {
    std::istringstream linestream(line);
    for (int i = 0; i < 13; i++) {
      linestream >> value;
    }
    linestream >> utime >> stime >> cutime >> cstime;
    return utime + stime + cutime + cstime;
  }

  return 0;
 }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { 
  return Jiffies() - IdleJiffies();
 }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { 
  std::ifstream filestream(kProcDirectory + kStatFilename);
  std::string line, cpu;

  long user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice;

  if (filestream.is_open() && std::getline(filestream, line)) {
    std::istringstream linestream(line);
    linestream >> cpu >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal >> guest >> guest_nice;
    return idle + iowait;
  }

  return 0;
 }

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { 
  vector<string> cpu_utilization;
  string line, key;
  std::ifstream filestream(kProcDirectory + kStatFilename);

  if (filestream.is_open() && std::getline(filestream, line)) {
    std::istringstream linestream(line);
    linestream >> key;
    while (linestream >> key) {
      cpu_utilization.push_back(key);
    }
  }

  return cpu_utilization;
 }

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  return System::TotalProcesses();
 }

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  return System::RunningProcesses();
 }

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) { 
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kCmdlineFilename);

  std::string line;

  if (filestream.is_open() && std::getline(filestream, line)) {
    return line;
  }

  return line;
 }

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) { 
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatusFilename);

  std::string line, key, value;

  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "VmSize:") {
        return value;
      }
    }
  }
  return value;
 }

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) { 
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatusFilename);

  std::string line, key, value;

  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "Uid:") {
        return value;
      }
    }
  }
  return value;
 }

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) { 
  string uid = Uid(pid);
  std::ifstream filestream(kPasswordPath);

  if (filestream.is_open()) {
    string line;
    while (std::getline(filestream, line)) {
      vector<string> tokens;
      std::istringstream linestream(line);
      string token;
      
      // Split the line into tokens
      while (std::getline(linestream, token, ':')) {
        tokens.push_back(token);
      }

      // Check if this line corresponds to the desired UID
      if (tokens.size() > 3 && tokens[2] == uid) {
        return tokens[0];
      }
    }
  }
  return "N/A";
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) { 
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatFilename);

  if (filestream.is_open()) {
    std::string line;
    if (std::getline(filestream, line)) {
      std::vector<std::string> tokens;
      std::istringstream linestream(line);
      std::string token;

      while (linestream >> token) {
        tokens.push_back(token);
      }

      if (tokens.size() > 21) {
        try {
          return std::stol(tokens[21]);
        } catch (const std::invalid_argument& e) {
          return 0;
        }
      }
    }
  }
  return 0;
 }
