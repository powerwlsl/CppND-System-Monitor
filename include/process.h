#ifndef PROCESS_H
#define PROCESS_H
#include "linux_parser.h"
#include <string>
#include <vector>


/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  int Pid();                               // TODO: See src/process.cpp
  std::string User();                      // TODO: See src/process.cpp
  std::string Command();                   // TODO: See src/process.cpp
  float CpuUtilization();                  // TODO: See src/process.cpp
  std::string Ram();                       // TODO: See src/process.cpp
  long int UpTime();                       // TODO: See src/process.cpp
  bool operator<(Process const& a) const;  // TODO: See src/process.cpp
  Process(int pid) : _pid(pid){
    //Pid();
    User();
    CpuUtilization();
    Ram();
    UpTime();
    Command();
  };
  // std::vector<int> _pids;
  // int _pid;

  // TODO: Declare any necessary private members
 private:
  //long int uptime_;
  //int _uptime;
  int _pid;
  //std::vector<int> _pids;
};

#endif