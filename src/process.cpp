#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include "process.h"
#include "linux_parser.h"
#include "format.h"
using std::string;
using std::to_string;
using std::vector;

// TODO: Return this process's ID
int Process::Pid() { return _pid; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() { 
    int uptime = UpTime();
    string line, pid, brackets, val1, val2, val3, val4, val5, val6, val7, val8, val9, val10, utime, stime, cutime, cstime, starttime, val16, val17, val18, val19, val20, val21, val22, val23;
    float ans ;
    std::ifstream filestream(LinuxParser::kProcDirectory + to_string(_pid) +  LinuxParser::kStatFilename);
    if (filestream.is_open()) {
        std::getline(filestream, line);
        std::istringstream linestream(line);
        linestream >> pid >> brackets >> val1 >> val2 >> val3 >> val4>> val5 >> val6 >> val7 >> val8>> val9 >> val10 >> utime >> stime>> cutime >> cstime>> starttime >> val16 >> val17 >> val18>> val19 >> val20 >> val21 >> val22 >> val23;
        float total_time, seconds;
        total_time = stof(utime) + stof(stime) + stof(cutime) + stof(cstime);
        seconds = uptime - stof(starttime);
        ans = total_time/seconds;
    }
    return ans;
}

// TODO: Return the command that generated this process
string Process::Command() {
    return LinuxParser::Command(_pid);
}

// TODO: Return this process's memory utilization
string Process::Ram() {
    return LinuxParser::Ram(_pid);
}

// TODO: Return the user (name) that generated this process
string Process::User() {
    return LinuxParser::User(_pid);
    }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() {
    return LinuxParser::UpTime(_pid);

    }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const { return true; }