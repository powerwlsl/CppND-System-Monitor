#ifndef SYSTEM_H
#define SYSTEM_H

#include <string>
#include <vector>

#include "process.h"
#include "processor.h"

class System {
 public:
  System();
  Processor& Cpu() { return cpu_; }
  std::vector<Process>& Processes() { return processes_; }
  void UpdateProcesses();
  float MemoryUtilization();       
  long UpTime();         
  int TotalProcesses();          
  int RunningProcesses();       
  std::string Kernel() { return kernel_; }   
  std::string OperatingSystem() {return os_; }

  // Kernel and OS won't change during the execution, therefore we store them as const attributes
 private:
  Processor cpu_ = Processor();
  std::vector<Process> processes_;
  const std::string kernel_;
  const std::string os_;
};

#endif