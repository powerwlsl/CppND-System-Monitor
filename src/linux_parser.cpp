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
string LinuxParser::OperatingSystem()
{
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open())
  {
    while (std::getline(filestream, line))
    {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value)
      {
        if (key == "PRETTY_NAME")
        {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel()
{
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open())
  {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids()
{
  vector<int> pids;
  DIR *directory = opendir(kProcDirectory.c_str());
  struct dirent *file;
  while ((file = readdir(directory)) != nullptr)
  {
    // Is this a directory?
    if (file->d_type == DT_DIR)
    {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit))
      {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// DONE: Read and return the system memory utilization
float LinuxParser::MemoryUtilization()
{
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  std::string line;
  std::string key, value, unit;
  float memTotal = 0.0, memFree = 0.0;
  if (stream.is_open())
  {
    while (std::getline(stream, line))
    {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value >> unit)
      {
        if (key == "MemTotal")
          memTotal = std::stof(value);
        if (key == "MemFree")
          memFree = std::stof(value);
      }
    }
  }

  return (memTotal - memFree) / memTotal;
}

// DONE: Read and return the system uptime
long LinuxParser::UpTime()
{
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  std::string line, upTime;
  if (stream.is_open())
  {
    if (std::getline(stream, line))
    {
      std::istringstream linestream(line);
      linestream >> upTime;
    }
  }
  return std::stol(upTime);
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies()
{
  vector<string> vals = LinuxParser::CpuUtilization();
  long total = 0;
  vector<CPUStates> cpuStates = {kUser_, kNice_, kSystem_, kIdle_, kIOwait_, kIRQ_, kSoftIRQ_, kSteal_};
  for (int i : cpuStates)
  {
    total += stol(vals[i]);
  };

  return total;
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid)
{
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  string line, value;
  vector<string> values;
  int count = 0;
  long sum = 0;
  if (stream.is_open())
  {
    std::getline(stream, line);
    std::istringstream linestream(line);
    while (linestream >> value)
    {
      count++;
      if (count == 13)
        sum += std::stol(value);
      if (count == 14)
        sum += std::stol(value);
      if (count == 15)
        sum += std::stol(value);
      if (count == 16)
      {
        sum += std::stol(value);
        return sum / sysconf(_SC_CLK_TCK);
      }
    }
  }
  return -1;
}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies()
{
  vector<string> jiffies = CpuUtilization();

  return stol(jiffies[CPUStates::kUser_]) + stol(jiffies[CPUStates::kNice_]) + stol(jiffies[CPUStates::kSystem_]) + stol(jiffies[CPUStates::kIRQ_]) + stol(jiffies[CPUStates::kSoftIRQ_]) + stol(jiffies[CPUStates::kSteal_]);
}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies()
{
  vector<string> jiffies = CpuUtilization();

  return stol(jiffies[CPUStates::kIdle_]) + stol(jiffies[CPUStates::kIOwait_]);
}

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization()
{
  std::ifstream stream(kProcDirectory + kStatFilename);
  std::string line, key, value;
  vector<string> vals;
  if (stream.is_open())
  {
    if (std::getline(stream, line))
    {
      std::istringstream linestream(line);
      linestream >> key;
      while (linestream >> value)
      {
        vals.push_back(value);
      }
    }
  }
  return vals;
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses()
{
  std::ifstream stream(kProcDirectory + kStatFilename);
  std::string line, key, value;
  int totalProcesses = 0;
  if (stream.is_open())
  {
    while (std::getline(stream, line))
    {
      std::istringstream linestream(line);
      if (linestream >> key >> value)
      {
        if (key == "processes")
        {
          totalProcesses = std::stoi(value);
          break;
        }
      }
    }
  }

  return totalProcesses;
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses()
{
  std::ifstream stream(kProcDirectory + kStatFilename);
  std::string line, key, value;
  int runningProcesses = 0;

  if (stream.is_open())
  {
    while (std::getline(stream, line))
    {
      std::istringstream linestream(line);
      if (linestream >> key >> value)
      {
        if (key == "procs_running")
          runningProcesses = std::stoi(value);
      }
    }
  }

  return runningProcesses;
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid)
{
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kCmdlineFilename);
  std::string line;
  if (stream.is_open())
  {
    if (std::getline(stream, line))
    {
      return line;
    }
  }

  return std::string();
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid)
{
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  std::string line, key, val;
  if (stream.is_open())
  {
    while (std::getline(stream, line))
    {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      if (linestream >> key >> val)
      {
        if (key == "VmSize")
        {
          return std::to_string(std::stol(val) / 1024);
        }
      }
    }
  }

  return string();
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid)
{
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  std::string line, key, val;
  if (stream.is_open())
  {
    while (std::getline(stream, line))
    {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);

      if (linestream >> key >> val)
      {
        if (key == "Uid")
        {
          return val;
        }
      }
    }
  }
  return string();
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid)
{
  std::ifstream stream(kPasswordPath);
  std::string line, name, x, id;
  if (stream.is_open())
  {
    while (std::getline(stream, line))
    {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      if (linestream >> name >> x >> id)
      {
        if (id == Uid(pid))
          return name;
      }
    }
  }
  return string();
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid)
{
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatFilename);
  std::string line, val;
  int count = 0;
  if (stream.is_open())
  {
    if (std::getline(stream, line))
    {
      std::istringstream linestream(line);
      while (linestream >> val)
      {
        count++;
        if (count == 22)
        {
          return LinuxParser::UpTime() - (std::stol(val) / static_cast<double>(sysconf(_SC_CLK_TCK)));
        }
      }
    }
  }
  return 0;
}
