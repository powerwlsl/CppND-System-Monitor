#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include "linux_parser.h"
#include "process.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process (int p) {
  pId_ = p;
}

// TODO: Return this process's ID
int Process::Pid() const { return pid_; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() {
    long sUpTime = LinuxParser::UpTime();
    long pUpTime = LinuxParser::UpTime(pId_);
    long totalTime = LinuxParser::ActiveJiffies(pId_);

    float elapTime = float(sUpTime) - (float(pUpTime)/sysconf(_SC_CLK_TCK));
    cpuUtil_ = 100*(float(totalTime)/sysconf(_SC_CLK_TCK))/elapTime;
    return cpuUtil_;
}

// TODO: Return the command that generated this process
string Process::Command() { return LinuxParser::Command(Pid()); }

// TODO: Return this process's memory utilization
string Process::Ram() { return LinuxParser::Ram(Pid()); }

// TODO: Return the user (name) that generated this process
string Process::User() { return LinuxParser::User(Pid()); }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::UpTime(Pid()); }

// https://knowledge.udacity.com/questions/96882
// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const {
    return this->cpuUtil_ < a.cpuUtil_;
}