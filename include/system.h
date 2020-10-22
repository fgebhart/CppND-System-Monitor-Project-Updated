#ifndef SYSTEM_H
#define SYSTEM_H

#include <string>
#include <vector>

#include "process.h"
#include "processor.h"

class System {
 public:
  Processor& Cpu();                     // See src/system.cpp
  std::vector<Process>& Processes();    // See src/system.cpp
  float MemoryUtilization() const;      // See src/system.cpp
  long UpTime() const;                  // See src/system.cpp
  int TotalProcesses() const;           // See src/system.cpp
  int RunningProcesses() const;         // See src/system.cpp
  std::string Kernel() const;           // See src/system.cpp
  std::string OperatingSystem() const;  // See src/system.cpp
  void Update();  // start reading data from the file system

  // Define any necessary private members
 private:
  Processor cpu_ = {};
  std::vector<Process> processes_ = {};
  long uptime_;
  int totalprocesses_;
  int runningprocesses_;
  std::string os_;
  float mem_;
  std::string kernel_;
};

#endif