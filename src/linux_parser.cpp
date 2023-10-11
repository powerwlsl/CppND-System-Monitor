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

vector<string> LinuxParser::ConvertFileToVector(string filename) {
  string line;
  vector<string> file_vector;
  string file_item;
  std::ifstream filestream(filename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    while (linestream >> file_item) {
      file_vector.emplace_back(file_item);
    }
  }
  filestream.close();
  return file_vector;
}

string LinuxParser::GetValueAtKey(string filename, string keyFilter) {
  string line;
  string key;
  string value;
  std::ifstream stream(filename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == keyFilter) {
          return value;
        }
      }
    }
  }
  stream.close();
  return value;
};
  
string LinuxParser::GetValueOfFile(string filename) {
  string line;
  string value;
  std::ifstream stream(filename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> value;
  }
  stream.close();
  return value;
};

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
  return pids;
}

float LinuxParser::MemoryUtilization() {
  string line;
  string key;
  string memory_item;
  std::map<string, long> memory_items;
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      key = line.substr(0, line.find(":"));
      std::istringstream linestream(line.substr(line.find(":") + 1, line.size() - 1));
      linestream >> memory_item;
      memory_items[key] = std::stol(memory_item);
    }
  }
  long total_memory = memory_items["MemTotal"];
  long free_memory = memory_items["MemFree"];
  long buffers = memory_items["Buffers"];
  long cached_memory = memory_items["Cached"] + memory_items["SReclaimable"] - memory_items["Shmem"];
  float memory_utilization = 1.0 * (total_memory - free_memory - (buffers + cached_memory)) / total_memory;
  return memory_utilization;
}

long LinuxParser::UpTime() { 
  string system_uptime = GetValueOfFile(kProcDirectory + kUptimeFilename);
  return std::stol(system_uptime);
}

long LinuxParser::Jiffies() { 
  return ActiveJiffies() + IdleJiffies();
}

long LinuxParser::ActiveJiffies(int pid) {
  vector<string> process_numbers = ConvertFileToVector(kProcDirectory + std::to_string(pid) + kStatFilename);
  long totaltime = 0.0;
  if (std::all_of(process_numbers[13].begin(), process_numbers[13].end(), isdigit))
    totaltime += std::stol(process_numbers[13]);
  if (std::all_of(process_numbers[14].begin(), process_numbers[14].end(), isdigit))
    totaltime += std::stol(process_numbers[14]);
  if (std::all_of(process_numbers[15].begin(), process_numbers[15].end(), isdigit))
    totaltime += std::stol(process_numbers[15]);
  if (std::all_of(process_numbers[16].begin(), process_numbers[16].end(), isdigit))
    totaltime += std::stol(process_numbers[16]);
  return totaltime;
}

long LinuxParser::ActiveJiffies() {
  vector<string> jiffies = CpuUtilization();
  long active_jiffies = std::stol(jiffies[CPUStates::kUser_]) + std::stol(jiffies[CPUStates::kNice_]) + std::stol(jiffies[CPUStates::kSystem_]) + std::stol(jiffies[CPUStates::kIRQ_]) + std::stol(jiffies[CPUStates::kSoftIRQ_]) + std::stol(jiffies[CPUStates::kSteal_]);
  return active_jiffies;
}

long LinuxParser::IdleJiffies() {
  vector<string> jiffies = CpuUtilization();
  long idle_jiffies = std::stol(jiffies[CPUStates::kIdle_]) + std::stol(jiffies[CPUStates::kIOwait_]);
  return idle_jiffies;
}

vector<string> LinuxParser::CpuUtilization() {
  vector<string> cpu_utilization = LinuxParser::ConvertFileToVector(kProcDirectory + kStatFilename);
  cpu_utilization.erase(cpu_utilization.begin());
  return cpu_utilization;
}

int LinuxParser::TotalProcesses() {
  return std::stoi(GetValueAtKey(kProcDirectory + kStatFilename, "processes"));
}

int LinuxParser::RunningProcesses() {
  return std::stoi(GetValueAtKey(kProcDirectory + kStatFilename, "procs_running"));
}

string LinuxParser::Command(int pid) { 
  string line;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kCmdlineFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    return line;
  }
  return string();
}

string LinuxParser::Ram(int pid) {
  string vmsize;
  string ram;
  string line;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      if (line.substr(0, line.find(":")) == "VmSize") {
        std::istringstream linestream(line);
        linestream >> vmsize >> ram;
        float ram_converted = KB_TO_MB * std::stof(ram);
        std::stringstream ram_converted_stream;
        ram_converted_stream << std::fixed << std::setprecision(1) << ram_converted;
        return ram_converted_stream.str();
      }
    }
  }
  return string();
}

string LinuxParser::Uid(int pid) {
  return GetValueAtKey(kProcDirectory + std::to_string(pid) + kStatusFilename, "Uid");
}

string LinuxParser::User(int pid) {
  string uid = LinuxParser::Uid(pid);
  string line;
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      if (line.find(":x:" + uid) > 0) {
        return line.substr(0, line.find(":x:"));
      }
    }
  }
  return string();
}

long LinuxParser::UpTime(int pid) {
  vector<string> process_numbers = ConvertFileToVector(kProcDirectory + std::to_string(pid) + kStatFilename);
  long uptime = std::stol(process_numbers[21]);
  long starttime = 0;
  try {
    starttime = uptime / sysconf(_SC_CLK_TCK);
  } 
  catch (...) {
    starttime = 0;
  }
  return starttime;
}
