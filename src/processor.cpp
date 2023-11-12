#include "processor.h"
#include "linux_parser.h"
using std::vector, std::string;
// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 
    std::vector<string> cpu_jiffies_vector = LinuxParser::CpuUtilization();
    float total_cpu_jiffies = 0;
    for (string jiffy: cpu_jiffies_vector)
    {
        total_cpu_jiffies+=std::stol(jiffy);
    }
    
    float ans = 100*LinuxParser::TotalProcesses()/ total_cpu_jiffies;
    return ans;

 }