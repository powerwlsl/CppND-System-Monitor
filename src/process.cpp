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

Process::Process(int proc){
    pid = proc;
    user = LinuxParser::User(proc);
    ram = LinuxParser::Ram(proc);
    command = LinuxParser::Command(proc);
    cpu_utilization = CpuUtilization();
    uptime = LinuxParser::UpTime(proc);
}

// TODO: Return this process's ID 
int Process::Pid() { return pid; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() {
    string line;
    string pid_, comm, state, ppid, pgrp, session, tt_nr, tpgid, flags, minflt, cminflt, majflt, cmajflt, priority, nice, num_threads, itrealvalue;
    long utime, stime, cutime, cstime, starttime;
    // source: https://man7.org/linux/man-pages/man5/proc.5.htmls
    std::ifstream filestream(LinuxParser::kProcDirectory + std::to_string(pid) + LinuxParser::kStatFilename);
    if (filestream.is_open()) {
        std::getline(filestream, line);
        std::istringstream linestream(line);
        linestream >> pid_ >> comm >> state >> ppid >> pgrp >> session >> tt_nr >> tpgid >> flags >> minflt >> cminflt >> majflt >> cmajflt >> utime >> stime >> cutime >> cstime >> priority >> nice >> num_threads >> itrealvalue >> starttime;
        long total_time = utime + stime + cutime + cstime;
        float seconds = (float) LinuxParser::UpTime() - ( (float) starttime / sysconf(_SC_CLK_TCK));
        cpu_utilization = 100 * ((total_time / sysconf(_SC_CLK_TCK)) / seconds);
        return cpu_utilization;
        // source: https://stackoverflow.com/questions/16726779/how-do-i-get-the-total-cpu-usage-of-an-application-from-proc-pid-stat/16736599#16736599
    }
    return -1;
}

// TODO: Return the command that generated this process
string Process::Command() { return command; }

// TODO: Return this process's memory utilization
string Process::Ram() { return ram; }

// TODO: Return the user (name) that generated this process
string Process::User() { return user; }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { return uptime; }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const { return (this->cpu_utilization < a.cpu_utilization); }