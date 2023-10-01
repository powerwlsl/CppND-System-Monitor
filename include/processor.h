#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <vector>
class Processor {
 public:
  float Utilization();
  std::vector<long> ParseCPUData();
 private:
  long idle_time = 0;
  long total_time = 0;
};

#endif
