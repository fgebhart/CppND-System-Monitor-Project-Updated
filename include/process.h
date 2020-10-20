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
  int Pid() const;
  std::string User() const;
  std::string Command() const;
  float CpuUtilization() const;
  std::string Ram() const;
  long int UpTime() const;
  bool operator<(Process const& a) const;
  std::string GetMemoryUtilizationOfProcess(int pid) const;
  std::string GetCommandOfProcess(int pid) const;

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