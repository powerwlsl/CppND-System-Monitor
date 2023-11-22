#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"

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
  float mem_stats[4]; // total, free, available, buffer
  string junk[4];
  string line;

  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()){
    for (int i = 0; i < 4; i++){
      std::getline(filestream, line);
      std::istringstream linestream(line);
      linestream >> junk[i] >> mem_stats[i];
    }
  }
  
  return (mem_stats[0] - mem_stats[1]) / mem_stats[0]; // Total used memory / total memory (Percentage in use)
  // Memory utilization formula: https://stackoverflow.com/questions/41224738/how-to-calculate-system-memory-usage-from-proc-meminfo-like-htop/41251290#41251290
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() {
  float uptime;
  string line;
  std::ifstream filestream(kProcDirectory + kUptimeFilename);
  if (filestream.is_open()){
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> uptime;
    return (long) uptime; //Truncates time 
  }
  return -1; // ERROR
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() {
  //user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice
  long jiffies[10];
  long sum = 0;
  string line, cpu;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if(filestream.is_open()){
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> cpu;
    int i = 0;
    while(linestream){
      linestream >> jiffies[i];
      sum += jiffies[i];
    }
  }
  return sum;
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) { return pid; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {
  //user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice
  //  *    *      *                    *      *        *
  long user, nice, system, idle, iowait, irq, softirq, steal; //, guest, guest_nice
  string line, cpu;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if(filestream.is_open()){
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> cpu >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal;
  }
  return user + nice + system + irq + softirq + steal;
}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
  //user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice
  //                     *      *
  long user, nice, system, idle, iowait, irq, softirq, steal; //, guest, guest_nice
  string line, cpu;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if(filestream.is_open()){
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> cpu >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal;
  }
  return idle + iowait;
}

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { return {}; }

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  string line;
  string key;
  int value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "processes") {
        return value;
      }
    }
  }
  return -1; // Error
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  string line;
  string key;
  int value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "procs_running") {
        return value;
      }
    }
  }
  return -1; // Error
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) {
  string line;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kCmdlineFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    return line;
  }
  return "[ERROR]";
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) {
  string line;
  string key;
  float value;
  string ram;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "VmSize:") {
          value /= 1000; // To Mb
          ram = std::to_string(value);
          return ram;
        }
      }
    }
  }
  return "[ERROR]";
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) {
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatusFilename);
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
  return "[ERROR]";
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) {
  string uid = LinuxParser::Uid(pid);
  string line;
  string key;
  string value;
  string temp;
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> value >> temp >> key) {
        if (key == uid) {
          return value;
        }
      }
    }
  }
  return "[ERROR]";
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) {
  string line;
  string pid_, comm, state, ppid, pgrp, session, tt_nr, tpgid, flags, minflt, cminflt, majflt, cmajflt, utime, stime, cutime, cstime, priority, nice, num_threads, itrealvalue;
  long clock_ticks;
  // source: https://man7.org/linux/man-pages/man5/proc.5.htmls
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> pid_ >> comm >> state >> ppid >> pgrp >> session >> tt_nr >> tpgid >> flags >> minflt >> cminflt >> majflt >> cmajflt >> utime >> stime >> cutime >> cstime >> priority >> nice >> num_threads >> itrealvalue >> clock_ticks;
    return clock_ticks / sysconf(_SC_CLK_TCK);
  }
  return -1;
}
