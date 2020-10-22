#include "processor.h"

#include <string>
#include <vector>

#include "linux_parser.h"

  using namespace LinuxParser;


// Return the aggregate CPU utilization
float Processor::Utilization() {
  std::vector<int> cpu_agg = LinuxParser::GetCPUData();
  user = cpu_agg[CPUStates::kUser_];
  nice = cpu_agg[CPUStates::kNice_];
  system = cpu_agg[CPUStates::kSystem_];
  idle = cpu_agg[CPUStates::kIdle_];
  iowait = cpu_agg[CPUStates::kIOwait_];
  irq = cpu_agg[CPUStates::kIRQ_];
  softirq = cpu_agg[CPUStates::kSoftIRQ_];
  steal = cpu_agg[CPUStates::kSteal_];

  // algorithm for computing the cpu utilization, taken from
  // https://stackoverflow.com/a/23376195/5384702
  int PrevIdle = previdle + previowait;
  int Idle = idle + iowait;

  int PrevNonIdle =
      prevuser + prevnice + prevsystem + previrq + prevsoftirq + prevsteal;
  int NonIdle = user + nice + system + irq + softirq + steal;

  int PrevTotal = PrevIdle + PrevNonIdle;
  int Total = Idle + NonIdle;

  // differentiate: actual value minus the previous one
  int totald = Total - PrevTotal;
  int idled = Idle - PrevIdle;

  float CPU_Percentage = float(totald - idled) / float(totald);

  // update previous values
  prevuser = user;
  prevnice = nice;
  prevsystem = system;
  previdle = idle;
  previowait = iowait;
  previrq = irq;
  prevsoftirq = softirq;
  prevsteal = steal;

  return CPU_Percentage;
}
