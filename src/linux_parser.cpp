#include "linux_parser.h"

#include <dirent.h>

#include <string>
#include <vector>

using std::string;
using std::vector;


vector<string> LinuxParser::GetCPUData() {
  vector<string> cpu_data;
  string value, line;
  std::ifstream filestream(LinuxParser::kProcDirectory +
                           LinuxParser::kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      if (line.find("cpu") != string::npos) {
        std::istringstream linestream(line);
        while (linestream >> value) {
          if (value != "cpu") {
            cpu_data.push_back(value);
          }
        }
        break;  // ensure to only collect the data of the first hit of "cpu"
      }
    }
  }
  return cpu_data;
}

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

string LinuxParser::GetNthValue(string path_to_file, int nth_value) {
  string item;
  string value;
  std::ifstream filestream(path_to_file);
  if (filestream.is_open()) {
    int counter = 1;
    while (std::getline(filestream, item, ' ')) {
      // get 22nd value of file, which holds the uptime
      if (counter == nth_value) {
        value = item;
      }
      counter += 1;
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

// Read and return the system memory utilization
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
