#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"

using std::string;
using std::to_string;
using std::vector;

// Process constructor
Process::Process(int pid) {
  pid_ = pid;
  uptime_ = LinuxParser::UpTime(pid_);
  long seconds = LinuxParser::UpTime() - uptime_;
  long totaltime = LinuxParser::ActiveJiffies(pid_);
  if (float(seconds) != 0)
    utilization_ = float(totaltime) / float(seconds);
  else
    utilization_ = 0;
  command_ = LinuxParser::Command(pid_);
  ram_ = std::stol(LinuxParser::Ram(pid_));
  user_ = LinuxParser::User(pid_);
}

// TODO: Return this process's ID
int Process::Pid() {
  return pid_;
  }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() { return utilization_; }

// TODO: Return the command that generated this process
string Process::Command() { return command_; }

// TODO: Return this process's memory utilization
string Process::Ram() { return to_string(ram_); }

// TODO: Return the user (name) that generated this process
string Process::User() { return user_; }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { return uptime_; }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
// I dont understand what this is
bool Process::operator<(Process const& a[[maybe_unused]]) const { 
  return true;
}