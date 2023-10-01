#ifndef PROCESS_H
#define PROCESS_H

#include <string>
#include "linux_parser.h"
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  Process(int pid) : pid(pid) {
    user = LinuxParser::User(pid);
    command = LinuxParser::Command(pid);
    ram = LinuxParser::Ram(pid);
    uptime = LinuxParser::UpTime(pid);
    cpu_utilization = CpuUtilization();
  }

  int Pid();                               // TODO: See src/process.cpp
  std::string User();                      // TODO: See src/process.cpp
  std::string Command();                   // TODO: See src/process.cpp
  float CpuUtilization() const;                  // TODO: See src/process.cpp
  std::string Ram();                       // TODO: See src/process.cpp
  long int UpTime();                       // TODO: See src/process.cpp
  bool operator<(Process const& a) const;  // TODO: See src/process.cpp

  // TODO: Declare any necessary private members
 private:
  int pid;
  float cpu_utilization;
  std::string user;
  std::string command;
  std::string ram;
  long int uptime;
};



#endif