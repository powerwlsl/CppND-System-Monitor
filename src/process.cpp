#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

// TODO: Return this process's ID
int Process::Pid() const { return m_pid; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() const { return float(LinuxParser::ActiveJiffies(m_pid) / 100.0) / float(LinuxParser::UpTime(m_pid)); }

// TODO: Return the command that generated this process
string Process::Command() { return LinuxParser::Command(m_pid); }

// TODO: Return this process's memory utilization
string Process::Ram() const { return LinuxParser::Ram(m_pid); }

// TODO: Return the user (name) that generated this process
string Process::User() { return LinuxParser::User(m_pid); }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::UpTime(m_pid); }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const &a) const { return CpuUtilization() < a.CpuUtilization(); }