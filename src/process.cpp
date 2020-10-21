#include "process.h"

#include <math.h>
#include <unistd.h>

#include <cctype>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

string ReduceStringLength(string input_string, int target_length) {
  int to_be_removed = input_string.size() - target_length;
  string output_string =
      input_string.substr(0, input_string.size() - to_be_removed);
  return output_string;
}

string Process::GetMemoryUtilizationOfProcess(int pid) const {
  float memory_utilization = LinuxParser::GetSecondPositionValue(
      LinuxParser::kProcDirectory + std::to_string(pid) +
          LinuxParser::kStatusFilename,
      "VmSize:");
  // turn kilo to megabytes
  memory_utilization = memory_utilization / 1000;
  // round to hundredths
  memory_utilization = floor(memory_utilization * 100 + 0.5) / 100;
  // limit string length for proper displaying
  return ReduceStringLength(std::to_string(memory_utilization), 6);
  ;
};

string Process::GetCommandOfProcess(int pid) const {
  string path_to_cmd_file = LinuxParser::kProcDirectory + std::to_string(pid) +
                            LinuxParser::kCmdlineFilename;
  return LinuxParser::GetEntireContentOfFile(path_to_cmd_file);
};

long int Process::GetUpTimeOfProcess(int pid) const {
  string value = LinuxParser::GetNthValue(LinuxParser::kProcDirectory +
                                              std::to_string(pid) +
                                              LinuxParser::kStatFilename,
                                          22);
  // turn clock ticks into seconds
  long int uptime = std::stoi(value) / sysconf(_SC_CLK_TCK);
  return uptime;
};

float Process::GetCpuUtilizationOfProcess(int pid) const {
  // system uptime
  string uptime = LinuxParser::GetNthValue(
      LinuxParser::kProcDirectory + LinuxParser::kUptimeFilename, 1);
  // #14 utime - CPU time spent in user code, measured in clock ticks
  string utime = LinuxParser::GetNthValue(LinuxParser::kProcDirectory +
                                              std::to_string(pid) +
                                              LinuxParser::kStatFilename,
                                          14);
  // #15 stime - CPU time spent in kernel code, measured in clock ticks
  string stime = LinuxParser::GetNthValue(LinuxParser::kProcDirectory +
                                              std::to_string(pid) +
                                              LinuxParser::kStatFilename,
                                          15);
  // #16 cutime - Waited-for children's CPU time spent in user code (in clock
  // ticks)
  string cutime = LinuxParser::GetNthValue(LinuxParser::kProcDirectory +
                                               std::to_string(pid) +
                                               LinuxParser::kStatFilename,
                                           16);
  // #17 cstime - Waited-for children's CPU time spent in kernel code (in clock
  // ticks)
  string cstime = LinuxParser::GetNthValue(LinuxParser::kProcDirectory +
                                               std::to_string(pid) +
                                               LinuxParser::kStatFilename,
                                           17);
  // #22 starttime - Time when the process started, measured in clock ticks
  string starttime = LinuxParser::GetNthValue(LinuxParser::kProcDirectory +
                                                  std::to_string(pid) +
                                                  LinuxParser::kStatFilename,
                                              22);
  // calculation
  float total_time = ::atof(utime.c_str()) + ::atof(stime.c_str());
  total_time = total_time + ::atof(cutime.c_str()) + ::atof(cstime.c_str());
  float seconds = ::atof(uptime.c_str()) -
                  (::atof(starttime.c_str()) / sysconf(_SC_CLK_TCK));
  float cpu_usage = 100 * ((total_time / sysconf(_SC_CLK_TCK)) / seconds);
  return cpu_usage;
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
  uptime_ = GetUpTimeOfProcess(pid);
  cpu_utilization_ = GetCpuUtilizationOfProcess(pid);
}

// TODO: Return this process's ID
int Process::Pid() const { return pid_; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() const { return cpu_utilization_; }

// TODO: Return the command that generated this process
string Process::Command() const { return command_; }

// TODO: Return this process's memory utilization
string Process::Ram() const { return ram_; }

// TODO: Return the user (name) that generated this process
string Process::User() const { return user_; }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() const { return uptime_; }

// Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process const& a) const {
  return a.cpu_utilization_ < cpu_utilization_ ? true : false;
}