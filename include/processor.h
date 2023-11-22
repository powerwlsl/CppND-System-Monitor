#ifndef PROCESSOR_H
#define PROCESSOR_H

#include "linux_parser.h"

class Processor {
 public:
  float Utilization();  // TODO: See src/processor.cpp

  // TODO: Declare any necessary private members
 private:
 long idle{LinuxParser::IdleJiffies()};
 long active{LinuxParser::ActiveJiffies()};
 long total{LinuxParser::Jiffies()};
 int iteration_counter = 1;
 int iteration_gap = 200;
};

#endif