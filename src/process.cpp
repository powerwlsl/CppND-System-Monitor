#include <unistd.h>
#include <string>

#include "process.h"
#include "linux_parser.h"

// Formula given in https://stackoverflow.com/questions/16726779/how-do-i-get-the-total-cpu-usage-of-an-application-from-proc-pid-stat/16736599#16736599
float Process::CpuUtilization() const { 
    long active_jiffies = LinuxParser::ActiveJiffies(pid_);
    long uptime = LinuxParser::UpTime(pid_);
    return (float) active_jiffies / sysconf(_SC_CLK_TCK) / uptime; 
}

std::string Process::Command() const { return LinuxParser::Command(pid_); }

std::string Process::Ram() const { return LinuxParser::Ram(pid_); }

std::string Process::User() const { return LinuxParser::User(pid_); }

long int Process::UpTime() const { return LinuxParser::UpTime(pid_); }

// Total ordering based on the process' CPU utilization
bool Process::operator<(Process const& a) const { 
    return CpuUtilization() > a.CpuUtilization(); 
}