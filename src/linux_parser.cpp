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
using std::stol;
using std::stoi;

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
  // ? Calculation : (Total memory - free memory) / Total Memory
  // read file /proc/meminfo
  string line;
  string utilize;
  float total_memory;
  float free_memory;
  std::ifstream memory(kProcDirectory + kMeminfoFilename);
  if (memory.is_open()) {
    while (std::getline(memory, line)) {
      std::istringstream streamingline(line); // a string class object which is used to stream the string into different
                                              // variables and similarly files can be stream into strings
      //std::istringstream in C++ allows performing insertion, extraction, and other operations
      streamingline >> utilize;
      if(utilize == "MemTotal") {
        streamingline >> total_memory;
      } else if (utilize == "MemAvailable") {
        streamingline >> free_memory;
        break;
      }
    }

  }

  
  return (total_memory - free_memory) / total_memory; 
  }

// TODO: Read and return the system uptime
long LinuxParser::UpTime() { 
  long uptime;
  string line;
  string checkUptime;

  std::ifstream utime(kProcDirectory + kUptimeFilename);
  if (utime.is_open()) {
    std::getline(utime, line);
    std::istringstream streamingUptime(line);
    streamingUptime >> checkUptime;
  }
  uptime = std::stol(checkUptime); //std::stol : This function converts the string, provided as an argument 
                                   //in the function call, to long int.It parses str interpreting its 
                                   //content as an integral number of the specified base, which is returned 
                                   //as a value of type long int. 



  return uptime; }

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { 
  return LinuxParser::ActiveJiffies() + LinuxParser::IdleJiffies(); 
  }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) { 
  long Total_time;
  string line;
  string jiffies;
  vector<string> Active_jiffies;
  std::ifstream streaming_jiffies(kProcDirectory + to_string(pid) + kStatFilename);
  if(streaming_jiffies.is_open()) {
    std::getline(streaming_jiffies,line);
    std::istringstream streamingLine(line);
    while(streamingLine >> jiffies) {
      Active_jiffies.push_back(jiffies);
    }
  }

  // https://stackoverflow.com/questions/16726779/how-do-i-get-the-total-cpu-usage-of-an-application-from-proc-pid-stat
  Total_time = stol(Active_jiffies[13]) + stol(Active_jiffies[14]) + 
                      stol(Active_jiffies[15]) + stol(Active_jiffies[16]); 
  
  return Total_time /sysconf(_SC_CLK_TCK);
  }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { 
  
  auto ActiveJiff = CpuUtilization();
  long CpuUtilized = stol(ActiveJiff[CPUStates::kUser_]) + stol(ActiveJiff[CPUStates::kNice_]) +
                     stol(ActiveJiff[CPUStates::kSystem_]) + stol(ActiveJiff[CPUStates::kIRQ_]) +
                     stol(ActiveJiff[CPUStates::kSoftIRQ_]) + stol(ActiveJiff[CPUStates::kSteal_]);


  return CpuUtilized; 
  }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { 

  auto IdleJiffies = CpuUtilization();
  long CPUutilized = stol(IdleJiffies[CPUStates::kIdle_]) + stol(IdleJiffies[CPUStates::kIOwait_]);
  
  
  return CPUutilized; 
  }

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { 
  string line, value, cpu;
  vector<string> cpuUtilited;
  std::ifstream streaming_cpu_jiffies(kProcDirectory + kStatFilename);
  if(streaming_cpu_jiffies.is_open()) {
    std::getline(streaming_cpu_jiffies, line);
    std::istringstream Read_stream(line);
    Read_stream >> cpu;
    while(Read_stream >> value) {
      cpuUtilited.push_back(value);
    }
  }
  
  return cpuUtilited; 
  }

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  string line, processess, value;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      while (linestream >> processess >> value) {
        if (processess == "processes") {
          return stoi(value);
        }
      }
    }
  }

  return 0; 
  }

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  string line, running_processes, value;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      while (linestream >> running_processes >> value) {
        if (running_processes == "procs_running") {
          return stoi(value);
        }
      }
    }
  }
  return 0; 
  }

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) {
  string command;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kCmdlineFilename);
  if (stream.is_open()) {
    std::getline(stream, command);
  }
  return command;
  }

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) { 
  string line, key, ram, value;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key) {
        if (key == "VmSize:") {
          linestream >> value;
          ram = to_string(stol(value) / 1024);
          break; 
        }
      }
    }
  }
  
  return ram; 
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) { 
  string line, key, uid, value;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key) {
        if (key == "Uid:") {
          linestream >> value;
          uid = to_string(stol(value) / 1024);
          break; 
        }
      }
    }
  }
  
  return uid; 
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) { 
  string line;
  string name, x, uid;
  std::ifstream stream(kPasswordPath);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> name >> x >> uid) {
        if (uid == LinuxParser::Uid(pid)) {
          return name;
        }
      }
    }
  }
  return string(); }

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) {
  string line, value;
  vector<string> values;
  long start_time = 0;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    while (linestream >> value) {
      values.push_back(value);
    }
  }
  try {
    start_time = stol(values[21]) / sysconf(_SC_CLK_TCK);
  } catch (...) {
    start_time = 0;
  }
  return start_time;
}