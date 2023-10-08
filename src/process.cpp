#include "process.h"

#include <unistd.h>

#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(size_t pid) { _pid = pid; }

int Process::Pid() { return _pid; }

float Process::CpuUtilization() {
  // Calculate active time, uptime, and utilization
  float activeTime = static_cast<float>(LinuxParser::ActiveJiffies(Pid())) / 100.0f;
  float uptime = static_cast<float>(LinuxParser::UpTime(Pid()));
  float utilization = activeTime / uptime;

  return utilization;
}

string Process::Command() { return LinuxParser::Command(_pid); }

string Process::Ram() const { return LinuxParser::Ram(_pid); }

string Process::User() { return LinuxParser::User(_pid); }

long int Process::UpTime() { return LinuxParser::UpTime(_pid); }

bool Process::operator<(Process const& a) const {
  return std::stof(Process::Ram()) < std::stof(a.Ram());
}