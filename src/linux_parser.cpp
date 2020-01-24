#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> kernel;
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
  float mem_total{0};
  float mem_free{0};
  float buffers{0};
  string token;
  std::ifstream stream(kProcDirectory + kmeminfoFilename);
  if (stream.is_open()) {
  }
  return 0.0;
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() {
  string token;
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream stream(line);
    if (stream >> token) {
      // Parse token as an int.
      return stoi(token);
    }
  }
  return 0;
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return UpTime() * sysconf(_SC_CLK_TCK); }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) {
  string line, token;
  vector<string> values;
  std::ifstream filestream(LinuxParser::kProcDirectory + to_string(pid) +
                           LinuxParser::kStatFilename);

  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    while (linestream >> token) {
      values.push_back(token);
    }
  }

  long jiffies{0};
  if (values.size() > 21) {
    long user = stol(values[13]);
    long kernel = stol(values[14]);
    long children_user = stol(values[15]);
    long children_kernel = stol(values[16]);
    jiffies = user + kernel + children_user + children_kernel;
  }
  return jiffies;
}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {
  vector<string> time = CpuUtilization();
  return (stol(time[CPUStates::kUser_]) + stol(time[CPUStates::kNice_]) +
          stol(time[CPUStates::kSystem_]) + stol(time[CPUStates::kIRQ_]) +
          stol(time[CPUStates::kSoftIRQ_]) + stol(time[CPUStates::kSteal_]) +
          stol(time[CPUStates::kGuest_]) + stol(time[CPUStates::kGuestNice_]));
}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
  vector<string> time = CpuUtilization();
  return (stol(time[CPUStates::kIdle_]) + stol(time[CPUStates::kIOwait_]));
}

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  string line, token;
  vector<string> values;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> token) {
        if (token == "cpu") {
          while (linestream >> token) values.push_back(token);
          return values;
        }
      }
    }
  }
  return values;
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  string key;
  string value;
  string line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream stream(line);
      while (stream >> key >> value) {
        if (key == "processes") {
          return stoi(value);
        }
      }
    }
  }
  return 0;
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  string line;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      string key, value;
      while (linestream >> key >> value) {
        if (key == "procs_running") {
          return stoi(value);
        }
      }
    }
  }
  return 0;
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) {
  string line;
  std::ifstream stream(LinuxParser::kProcDirectory + to_string(pid) +
                       LinuxParser::kCmdlineFilename);
  if (stream.is_open()) {
    string line;
    std::getline(stream, line);
    return line;
  }
  return "";
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) {
  string token;
  std::ifstream stream(LinuxParser::kProcDirectory + to_string(pid) +
                       LinuxParser::kStatusFilename);
  if (stream.is_open()) {
    while (stream >> token) {
      if (token == "VmSize:") {
        if (stream >> token) return std::to_string(stoi(token) / 1024);
      }
    }
  }
  return string("0");
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) {
  string token;
  std::ifstream stream(LinuxParser::kProcDirectory + to_string(pid) +
                       LinuxParser::kStatusFilename);
  if (stream.is_open()) {
    while (stream >> token) {
      if (token == "Uid:") {
        if (stream >> token) return token;
      }
    }
  }
  return string("");
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) {
  std::ifstream stream(LinuxParser::kPasswordPath);
  if (stream.is_open()) {
    string line;
    string token = "x:" + Uid(pid);
    while (std::getline(stream, line)) {
      auto marker = line.find(token);
      if (marker != string::npos) {
        return line.substr(0, marker - 1);
      }
    }
  }
  return "0";
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) { 
  string token;
  long int time{0};
  std::ifstream stream(LinuxParser::kProcDirectory + to_string(pid) +
                       LinuxParser::kStatFilename);
  if (stream.is_open()) {
    for (int i = 0; stream >> token; ++i)
      if (i == 13) {
        long int time{stol(token)};
        time /= sysconf(_SC_CLK_TCK);
        return time;
      }
  }
  return time;
 }