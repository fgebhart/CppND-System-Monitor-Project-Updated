#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <vector>
#include <string>


class Processor {
 public:
  std::vector<std::string> GetCPUData(std::string which_cpu);
  float Utilization();  // See src/processor.cpp

  // Declare any necessary private members
 private:
 // cpu stat values
 int idle, iowait, user, nice, system, irq, softirq, steal;
 // previous cpu stat values
 int previdle, previowait, prevuser, prevnice, prevsystem, previrq, prevsoftirq, prevsteal;
};

#endif