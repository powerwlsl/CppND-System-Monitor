#include <string>
#include <vector>
#include <algorithm>

#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"

System::System() : kernel_(LinuxParser::Kernel()), 
                   os_ (LinuxParser::OperatingSystem()) {
    UpdateProcesses();
}

// Updates the current list of processes, sorted by CPU utilization
void System::UpdateProcesses() {
    processes_.clear();
    auto pids = LinuxParser::Pids();
    for (int pid : pids) {
        processes_.emplace_back(Process(pid));
    }
    std::sort(processes_.begin(), processes_.end());
}

float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

long int System::UpTime() { return LinuxParser::UpTime(); }