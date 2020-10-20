#ifndef PROCESS_H
#define PROCESS_H

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  // getter functions
  int Pid();
  std::string User();
  std::string Command();
  float CpuUtilization();
  std::string Ram();
  long int UpTime();
  bool operator<(Process const& a) const;
  std::string GetMemoryUtilizationByPid(int pid);

  // setter functions
  void GatherInfo(int pid);

  // private members
 private:
  int pid_;
  std::string user_;
  std::string command_;
  float cpu_utilization_;
  std::string ram_;
  long int uptime_;
};

#endif