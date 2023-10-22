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
float LinuxParser::MemoryUtilization(){
  string line, word;
  float memTotal;
  float memAva;
  std::ifstream stream("/proc/meminfo");
  if (stream.is_open()) {
    while (std::getline(stream, line)) 
    {
      std::istringstream lstream(line);
      lstream >> word;
      if (word == "MemTotal:")     
        lstream >> memTotal;
      else if (word == "MemAvailable:") {       
        lstream >> memAva;
        break;
      }
    }
    return (memTotal - memAva)  / memTotal;
  }
  return 0.0; 
  }

// TODO: Read and return the system uptime
long LinuxParser::UpTime() { 
  string line, word;
  std::ifstream stream("/proc/uptime");
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream uptimeStream(line);
    uptimeStream >> word;
  }
  return std::stol(word);
 }

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return ActiveJiffies() + IdleJiffies(); }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) {
  string line, word;
  vector<string> words;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    while (linestream >> word) {
      words.push_back(word);
    }
  }
  long utime = 0, stime = 0 , cutime = 0, cstime = 0;
  if (std::all_of(words[13].begin(), words[13].end(), isdigit))
    utime = stol(words[13]);
  if (std::all_of(words[14].begin(), words[14].end(), isdigit))
    stime = stol(words[14]);
  if (std::all_of(words[15].begin(), words[15].end(), isdigit))
    cutime = stol(words[15]);
  if (std::all_of(words[16].begin(), words[16].end(), isdigit))
    cstime = stol(words[16]);

  return (utime + stime + cutime + cstime) / sysconf(_SC_CLK_TCK);
}
// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {
  auto jiffies = CpuUtilization();

  return stol(jiffies[0]) + stol(jiffies[1]) + stol(jiffies[2]) + stol(jiffies[5]) + stol(jiffies[6]) + stol(jiffies[7]);
}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { 
  auto jiffies = CpuUtilization();
  return stol(jiffies[3]) + stol(jiffies[4]);
 }

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { 
  string line,garbage, word;
  vector<string> jiffies;
  std::ifstream stream("/proc/stat");
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> garbage;
    while (linestream >> word) {
      jiffies.push_back(word);
    }
  }
  return jiffies;
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  string line, word, result;
  std::ifstream stream("/proc/stat");
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> word;
      if (word == "processes") {
        linestream >> result;
        break;
      }
    }
  return std::stol(result);
  }
}
// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
    string line, word, result;
  std::ifstream stream("/proc/stat");
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> word;
      if (word == "procs_running") {
        linestream >> result;
        break;
      }
    }
  return std::stol(result);
  }
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) { 
  string line, word;
  std::ifstream stream("/proc/" + to_string(pid) + "/cmdline");
  if (stream.is_open()) 
  {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> word;
  }
  return word; 
  }

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid[[maybe_unused]]) {
{ 
    string line, word, result;
    std::ifstream stream("/proc/" + to_string(pid) + "/status");
    if (stream.is_open()) {
      while (std::getline(stream, line)) {
        std::istringstream linestream(line);
        linestream >> word;
        if (word == "VmSize:") {
          linestream >> result;
          return result;
        }
      }
    }
  }
}
// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) {
  // Read and return the user ID associated with a process
  string line, word, result;
  std::ifstream stream("/proc/" + to_string(pid) + "/status");
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> word;
      if (word == "Uid:") {
        linestream >> result;
        break;
      }
    }
  }
  return result;
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) {
    // Read and return the user associated with a process
  string uid = Uid(pid);
  string line;
  string id, garbage, temp;
  string name;
  std::ifstream stream("/etc/passwd");
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> name >> garbage >> id;
      if (id == uid) {
        break;
      }
    }
  }
  return name;
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) { 
  string line, word;
  vector<string> values;
  long starttime = 0;
  std::ifstream stream("/proc/" + to_string(pid) + "/stat");
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    while (linestream >> word) {
      values.push_back(word);
    }
  }
  try {
    starttime = stol(values[21]) / sysconf(_SC_CLK_TCK);
  } catch (...) {
    starttime = 0;
  }
  return starttime;
}
