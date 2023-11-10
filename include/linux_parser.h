#ifndef SYSTEM_PARSER_H
#define SYSTEM_PARSER_H

#include <fstream>
#include <regex>
#include <string>

namespace LinuxParser {
// Paths
const std::string kProcDirectory{"/proc"};
const std::string kCmdlineFilename{"/cmdline"};
const std::string kCpuinfoFilename{"/cpuinfo"};
const std::string kStatusFilename{"/status"};
const std::string kStatFilename{"/stat"};
const std::string kUptimeFilename{"/uptime"};
const std::string kMeminfoFilename{"/meminfo"};
const std::string kVersionFilename{"/version"};
const std::string kOSPath{"/etc/os-release"};
const std::string kPasswordPath{"/etc/passwd"};

// System
float MemoryUtilization();
long UpTime();
std::vector<int> Pids();
int TotalProcesses();
int RunningProcesses();
std::string OperatingSystem();
std::string Kernel();

// CPU
enum CPUStates {
  kUser_ = 1,
  kNice_ = 2,
  kSystem_ = 4,
  kIdle_ = 8,
  kIOwait_ = 16,
  kIRQ_ = 32,
  kSoftIRQ_ = 64,
  kSteal_ = 128,
  kGuest_ = 256,
  kGuestNice_ = 512
};
const CPUStates kAllStates[] = {kUser_, kNice_, kSystem_, kIdle_, kIOwait_, kIRQ_, kSoftIRQ_, kSteal_, kGuest_, kGuestNice_};
const int kAllIdle = kIdle_ | kIOwait_;
const int kAllActive = kUser_ | kNice_ | kSystem_ | kIRQ_ | kSoftIRQ_ | kSteal_;
const int kAllJiffies = 255;
std::map<CPUStates, long> CpuUtilization();
long SystemJiffiesFiltered(const int filter);
long Jiffies();
long ActiveJiffies();
long ActiveJiffies(int pid);
long IdleJiffies();

// Processes
std::string Command(int pid);
std::string Ram(int pid);
std::string Uid(int pid);
std::string User(int pid);
long int UpTime(int pid);
};  // namespace LinuxParser

#endif