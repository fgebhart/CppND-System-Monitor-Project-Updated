#include "system.h"

#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "linux_parser.h"
#include "process.h"
#include "processor.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

// start reading data from the file system and cache the values in the System
// class' member variables
void System::Update() {
  uptime_ = LinuxParser::UpTime();
  totalprocesses_ = LinuxParser::TotalProcesses();
  runningprocesses_ = LinuxParser::RunningProcesses();
  os_ = LinuxParser::OperatingSystem();
  mem_ = LinuxParser::MemoryUtilization();
  kernel_ = LinuxParser::Kernel();
  // gather information on processes
  processes_ = {};  // reinitialize vector of processes
  for (auto pid : LinuxParser::Pids()) {
    Process process;
    process.GatherInfo(pid);
    processes_.push_back(process);
  }
  // sort the vector of processes using the overloaded < operator and a lambda
  // expression
  std::sort(processes_.begin(), processes_.end(),
            [&](const auto p1, const auto p2) { return p1 < p2; });
}

// Getter functions of the systemclass to return the cached values:
// Return the system's CPU
Processor& System::Cpu() { return cpu_; }

// Return a container composed of the system's processes
vector<Process>& System::Processes() { return processes_; }

// Return the system's kernel identifier (string)
std::string System::Kernel() { return kernel_; }

// Return the system's memory utilization
float System::MemoryUtilization() { return mem_; }

// Return the operating system name
std::string System::OperatingSystem() { return os_; }

// Return the number of processes actively running on the system
int System::RunningProcesses() { return runningprocesses_; }

// Return the total number of processes on the system
int System::TotalProcesses() { return totalprocesses_; }

// Return the number of seconds since the system started running
long int System::UpTime() { return uptime_; }