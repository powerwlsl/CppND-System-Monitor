#ifndef PROCESSOR_H
#define PROCESSOR_H
#include "linux_parser.h"
using std::string;
using std::vector;
using namespace LinuxParser;

class Processor {
 public:
  float Utilization();  // TODO: See src/processor.cpp

  // TODO: Declare any necessary private members
 private:
  float prevIdle = 0.0;
  float prevNonIdle = 0.0;
  float prevTotal = 0.0;
};

#endif