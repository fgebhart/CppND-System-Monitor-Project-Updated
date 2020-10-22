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
  std::string Status() const;
  long int UpTime() const;
  bool operator<(Process const& a) const;
  // setter functions
  void GatherInfo(int& pid);

  // private members
 private:
  int pid_;
  std::string user_;
  std::string command_;
  float cpu_utilization_;
  std::string ram_;
  std::string status_;
  long int uptime_;
};

std::string ReduceStringLength(std::string& input_string, int target_length);

#endif