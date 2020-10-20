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

string Process::GetMemoryUtilizationOfProcess(int pid) const {
  int memory_utilization = LinuxParser::GetSecondPositionValue(
      LinuxParser::kProcDirectory + std::to_string(pid) +
          LinuxParser::kStatusFilename,
      "VmSize:");
  return std::to_string(memory_utilization / 1000);
};

string Process::GetCommandOfProcess(int pid) const {
  string path_to_cmd_file = LinuxParser::kProcDirectory + std::to_string(pid) +
                            LinuxParser::kCmdlineFilename;
  return LinuxParser::GetEntireContentOfFile(path_to_cmd_file);
};

void Process::GatherInfo(int pid) {
  pid_ = pid;
  int uid = LinuxParser::GetSecondPositionValue(
      LinuxParser::kProcDirectory + std::to_string(pid) +
          LinuxParser::kStatusFilename,
      "Uid:");
  user_ = LinuxParser::GetUserNameByUid(std::to_string(uid));
  ram_ = GetMemoryUtilizationOfProcess(pid);
  command_ = GetCommandOfProcess(pid);
}

// TODO: Return this process's ID
int Process::Pid() const { return pid_; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() const { return 0; }

// TODO: Return the command that generated this process
string Process::Command() const { return command_; }

// TODO: Return this process's memory utilization
string Process::Ram() const { return ram_; }

// TODO: Return the user (name) that generated this process
string Process::User() const { return user_; }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() const { return 0; }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a [[maybe_unused]]) const {
  return true;
}