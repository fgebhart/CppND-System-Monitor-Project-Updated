#include "process.h"

#include <unistd.h>

#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

string Process::GetMemoryUtilizationByPid(int pid) {
  int memory_utilization = LinuxParser::GetSecondPositionValue(
      LinuxParser::kProcDirectory + std::to_string(pid) +
          LinuxParser::kStatusFilename,
      "VmSize:");
  return std::to_string(memory_utilization / 1000);
};

void Process::GatherInfo(int pid) {
  pid_ = pid;
  int uid = LinuxParser::GetSecondPositionValue(
      LinuxParser::kProcDirectory + std::to_string(pid) +
          LinuxParser::kStatusFilename,
      "Uid:");
  user_ = LinuxParser::GetUserNameByUid(std::to_string(uid));
  ram_ = GetMemoryUtilizationByPid(pid);
}

// TODO: Return this process's ID
int Process::Pid() { return pid_; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() { return 0; }

// TODO: Return the command that generated this process
string Process::Command() { return string(); }

// TODO: Return this process's memory utilization
string Process::Ram() { return ram_; }

// TODO: Return the user (name) that generated this process
string Process::User() { return user_; }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { return 0; }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a [[maybe_unused]]) const {
  return true;
}