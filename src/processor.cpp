#include "processor.h"

#include <string>
#include <vector>

#include "linux_parser.h"

/**
 * Get CPU data from kStatFilename.
 *
 * @param which_cpu: could be e.g. 'cpu' for the aggregate of all cpus or
 * 'cpu0', 'cpu1', ... for individual cpus.
 * @return a vector containing the cpu data.
 */
std::vector<std::string> Processor::GetCPUData(std::string which_cpu) {
  std::vector<std::string> cpu_data;
  std::string value, line;
  std::ifstream filestream(LinuxParser::kProcDirectory +
                           LinuxParser::kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      if (line.find(which_cpu) != std::string::npos) {
        std::istringstream linestream(line);
        while (linestream >> value) {
          if (value != which_cpu) {
            cpu_data.push_back(value);
          }
        }
        break;  // ensure to only collect the data of the first hit for given
                // 'which_cpu'
      }
    }
  }
  return cpu_data;
}

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() {
  std::vector<std::string> cpu_agg = GetCPUData("cpu");
  user = std::stoi(cpu_agg[0]);
  nice = std::stoi(cpu_agg[1]);
  system = std::stoi(cpu_agg[2]);
  idle = std::stoi(cpu_agg[3]);
  iowait = std::stoi(cpu_agg[4]);
  irq = std::stoi(cpu_agg[5]);
  softirq = std::stoi(cpu_agg[6]);
  steal = std::stoi(cpu_agg[7]);

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
