#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"
#include "format.h"

using std::stof;
using std::stol;
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
  string line, key, value;
  float memUtil;
  float mem_total, mem_free;
  //float mem_available, mem_buffers;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
    while (std::getline(stream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      //MemUtil = MemTotal - MemFree - MemAvailable - Buffers
      while (linestream >> key >> value) {
        if (key == "MemTotal") {
          mem_total = std::stof(value);
          //memUtil += std::stof(value);
        }
        if (key == "MemFree") {
          mem_free = std::stof(value);
        }
        // if (key == "MemAvailable") {
        //   mem_available = std::stof(value);
        // }
        // if (key == "Buffers") {
        //   mem_buffers = std::stof(value);
        // }
      }
      memUtil = (mem_total-mem_free)/mem_total ;
    }
  return memUtil;  
  }

// TODO: Read and return the system uptime
long LinuxParser::UpTime() { 
  long int time;
    string uptime, idleTime;
    string line;
    std::ifstream stream(LinuxParser::kProcDirectory + LinuxParser::kUptimeFilename);
    if (stream.is_open()) {
        std::getline(stream, line);
        std::istringstream linestream(line);
        while(linestream >> uptime >> idleTime){            
            time = std::stol(uptime);}
        }       
  return time;
  }

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() {
  string line, cpu, val_user, val_nice, val_system, val_idle, val_iowait, val_irq, val_softirq, val8, val9, val10;
  long int total_jiffies = 0;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> cpu >> val_user>>val_nice>>val_system>>val_idle>>val_iowait>>val_irq>>val_softirq>>val8>>val9>>val10;
    if(cpu == "cpu"){
    total_jiffies += stol(val_user)+ stol(val_nice)+ stol(val_system)+ stol(val_idle)+ stol(val_iowait)+ stol(val_irq)+ stol(val_softirq)+ stol(val8)+ stol(val9)+ stol(val10);}
  }
  return total_jiffies;
  }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { 
  string line, cpu, val_user, val_nice, val_system, val_idle, val_iowait, val_irq, val_softirq, val8, val9, val10;
  long int total_jiffies = 0;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> cpu >> val_user>>val_nice>>val_system>>val_idle>>val_iowait>>val_irq>>val_softirq>>val8>>val9>>val10;
    if(cpu == "cpu"){
    total_jiffies += stol(val_user)+ stol(val_nice)+ stol(val_system);}
  }
  return total_jiffies; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { 
  string line, cpu, val_user, val_nice, val_system, val_idle, val_iowait, val_irq, val_softirq, val8, val9, val10;
  long int total_jiffies = 0;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> cpu >> val_user>>val_nice>>val_system>>val_idle>>val_iowait>>val_irq>>val_softirq>>val8>>val9>>val10;
    if(cpu == "cpu"){
    total_jiffies += stol(val_idle);}
  }
  return total_jiffies;
  }

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { 
  //100*process_jiffies/total_jiffies
  vector<string> cpu_utilization_vec;
  string line, cpu, val_user, val_nice, val_system, val_idle, val_iowait, val_irq, val_softirq, val_steal, val_guest, val_guest_nice;
  // long int total_jiffies = 0;
  string cpu_line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> cpu >> val_user>>val_nice>>val_system>>val_idle>>val_iowait>>val_irq>>val_softirq>>val_steal>>val_guest>>val_guest_nice;
    linestream >> cpu_line;
    if (cpu == "cpu"){
    cpu_utilization_vec.push_back(val_user);
    cpu_utilization_vec.push_back(val_nice);
    cpu_utilization_vec.push_back(val_system);
    cpu_utilization_vec.push_back(val_idle);
    cpu_utilization_vec.push_back(val_iowait);
    cpu_utilization_vec.push_back(val_irq);
    cpu_utilization_vec.push_back(val_softirq);
    cpu_utilization_vec.push_back(val_steal);
    cpu_utilization_vec.push_back(val_guest);
    cpu_utilization_vec.push_back(val_guest_nice);
    }
    }
  return cpu_utilization_vec;
  }

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  string line, key, value;
  float totalProc;
  std::ifstream stream(kProcDirectory + kStatFilename);
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "processes") {
          totalProc = std::stof(value);
        }
      }
    }
  return totalProc; 
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  string line, key, value;
  float runningProc;
  std::ifstream stream(kProcDirectory + kStatFilename);
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "procs_running") {
          runningProc = std::stof(value);
        }
      }
    }
  return runningProc; 

}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) {
  string ans, line;
  std::ifstream filestream(LinuxParser::kProcDirectory + to_string(pid) +  LinuxParser::kCmdlineFilename);
  if (filestream.is_open()) {
      std::getline(filestream, line);
      std::istringstream linestream(line);
      linestream >> line;
      ans = line;
  }
  return ans;

}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) {
  string line, key, value;
  long mem_util;
  std::ifstream stream(LinuxParser::kProcDirectory + to_string(pid) + LinuxParser::kStatusFilename);
  while (std::getline(stream, line)) {
    std::istringstream linestream(line);
    while (linestream >> key >> value) {
      if (key == "VmSize:") {
        mem_util = std::stol(value)/1000;
      }
    }
  }
  return to_string(mem_util);

}

// TODO: Read and return the user ID associated with a process UID
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) {
  string line, key, value, uid1, uid2, uid3, uid4, ans;
  std::ifstream filestream(LinuxParser::kProcDirectory + to_string(pid) +  LinuxParser::kStatusFilename);
  if (filestream.is_open()) {
        while (std::getline(filestream, line)) {
        std::replace(line.begin(), line.end(), ' ', '_');
        std::replace(line.begin(), line.end(), '=', ' ');
        std::replace(line.begin(), line.end(), '"', ' ');
        std::istringstream linestream(line);
        while (linestream >> key >> uid1 >> uid2 >> uid3 >> uid4) {
            if (key == "Uid:") {
               return uid1;
        }
      }
    }    
  }
  return ans;
}

// TODO: Read and return the user associated with a process USERNAME
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) {
  string uid = Uid(pid);
  string line, name, passwd, gid, value, val, ans ;
  std::ifstream filestream(LinuxParser::kPasswordPath);
  if (filestream.is_open()) {
        while (std::getline(filestream, line)) {
        std::replace(line.begin(), line.end(), ':', ' ');
        std::istringstream linestream(line);
        while (linestream >> name >> passwd >> gid >> value >> val) {
            if (gid == uid) {
               return name;
          }
        }
      }
    }
  return ans;
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) { 
  string line, id, brackets, val1, val2, val3, val4, val5, val6, val7, val8, val9, val10, utime, stime, cutime, cstime, starttime, val16, val17, val18, val19, val20, val21, val22, val23;
  long ans;
  std::ifstream filestream(LinuxParser::kProcDirectory + to_string(pid) +  LinuxParser::kStatFilename);
  if (filestream.is_open()) {
      std::getline(filestream, line);
      std::istringstream linestream(line);
      linestream >> id >> brackets >> val1 >> val2 >> val3 >> val4>> val5 >> val6 >> val7 >> val8>> val9 >> val10 >> utime >> stime>> cutime >> cstime>> starttime >> val16 >> val17 >> val18>> val19 >> val20 >> val21 >> val22 >> val23;
      ans = stol(val20);
  }
  return ans;
  
}
