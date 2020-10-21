#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <string>
#include <vector>

class Processor {
 public:
  std::vector<std::string> GetCPUData();
  float Utilization();  // See src/processor.cpp

  // Declare any necessary private members
 private:
  // cpu stat values
  int idle, iowait, user, nice, system, irq, softirq, steal;
  // previous cpu stat values
  int previdle, previowait, prevuser, prevnice, prevsystem, previrq,
      prevsoftirq, prevsteal;
};

#endif