#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>
#include "process.h"

using std::string;
using std::to_string;
using std::vector;

// TODO: Return this process's ID
int Process::Pid() { 
  return pid;
 }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() const { 
  long total_time = LinuxParser::ActiveJiffies(pid);
  long start_time = LinuxParser::UpTime(pid);
  long uptime = LinuxParser::UpTime();
  long seconds = uptime - (start_time / sysconf(_SC_CLK_TCK));

  float cpu_usage = (total_time / sysconf(_SC_CLK_TCK)) / (float)seconds;
  return cpu_usage;
}


// TODO: Return the command that generated this process
string Process::Command() { 
  return command;
 }

// TODO: Return this process's memory utilization
string Process::Ram() { 
  return ram;
 }

// TODO: Return the user (name) that generated this process
string Process::User() { 
  return user;
 }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { 
  return uptime;
 }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const { 
  return CpuUtilization() < a.CpuUtilization();
 }