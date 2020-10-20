#include "linux_parser.h"

#include <dirent.h>
#include <unistd.h>

#include <iostream>
#include <string>
#include <vector>

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// get the value to a given key in a file
string LinuxParser::FindValueForKeyInFile(string path_to_file,
                                          string lookup_key) {
  string line;
  string key;
  string value;
  std::ifstream filestream(path_to_file);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == lookup_key) {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

int LinuxParser::GetSecondPositionValue(string path_to_file,
                                        string lookup_key) {
  string line;
  string key;
  int value;
  std::ifstream filestream(path_to_file);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      if (line.find(lookup_key) != std::string::npos) {
        std::istringstream linestream(line);
        linestream >> key >> value;
      }
    }
  }
  return value;
}

string LinuxParser::GetUserNameByUid(string lookup_uid) {
  string username;
  string line;
  string key;
  string separator;
  string uid;
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> username >> separator >> uid) {
        if (uid == lookup_uid) {
          return username;
        }
      }
    }
  }
  return username;
}

string LinuxParser::GetEntireContentOfFile(string path_to_file) {
  std::ifstream input(path_to_file);
  std::stringstream sstr;
  while (input >> sstr.rdbuf())
    ;
  return sstr.str();
}

string LinuxParser::OperatingSystem() {
  return FindValueForKeyInFile(kOSPath, "PRETTY_NAME");
}

// An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  int MemTotal = LinuxParser::GetSecondPositionValue(
      kProcDirectory + kMeminfoFilename, "MemTotal:");
  int MemFree = LinuxParser::GetSecondPositionValue(
      kProcDirectory + kMeminfoFilename, "MemFree:");
  return float(MemTotal - MemFree) / float(MemTotal);
}

// Read and return the system uptime
long LinuxParser::UpTime() {
  long uptime, idletime;
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> uptime >> idletime;
  }
  return uptime;
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid [[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { return {}; }

// Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  return LinuxParser::GetSecondPositionValue(kProcDirectory + kStatFilename,
                                             "processes");
}

// Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  return LinuxParser::GetSecondPositionValue(kProcDirectory + kStatFilename,
                                             "procs_running");
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid [[maybe_unused]]) { return string(); }

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid [[maybe_unused]]) { return string(); }

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid [[maybe_unused]]) { return string(); }

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid [[maybe_unused]]) { return string(); }

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid [[maybe_unused]]) { return 0; }