#ifndef PROCESSOR_H
#define PROCESSOR_H

#include "linux_parser.h"

// class Processor {
//  public:
//   float Utilization();  // TODO: See src/processor.cpp

//   // TODO: Declare any necessary private members
//  private:
// };
class Processor {
 public:
  float Utilization();

  void UpdateStats(long idle, long active, long total);

 private:
  long m_idle;
  long m_active;
  long m_total;
};

#endif