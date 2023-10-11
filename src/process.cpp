#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"

using std::string;
using std::to_string;
using std::vector;

int Process::Pid() {
  return pid_;
}

float Process::CpuUtilization() const {
  float active_jiffies_seconds = LinuxParser::ActiveJiffies(pid_) / (float) sysconf(_SC_CLK_TCK);
  long process_start_seconds = LinuxParser::UpTime() - LinuxParser::UpTime(pid_);
  float cpu_usage = process_start_seconds != 0 ? (active_jiffies_seconds / (float) process_start_seconds) : 0.0;
  return cpu_usage;
}

string Process::Command() {
  return LinuxParser::Command(Pid());
}

string Process::Ram() {
  return LinuxParser::Ram(Pid());
}

string Process::User() {
  return LinuxParser::User(Pid());
}

long int Process::UpTime() {
  return LinuxParser::UpTime() - LinuxParser::UpTime(Pid());
}

bool Process::operator<(Process const& a) const {
  return this->CpuUtilization() < a.CpuUtilization();
}