#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>

#include <iostream>

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
  int MemTotal, MemFree;
  string line;
  string key, value;

  std::ifstream filestream(kProcDirectory + kMeminfoFilename);

  if (filestream.is_open()) {

    while(std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');

      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "MemTotal") {
          MemTotal = stoi(value);
        }

        else if (key == "MemFree") {
          MemFree = stoi(value);
        }
      }
    }

  }

  return ((MemTotal - MemFree)*1.0) / MemTotal;
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() {
  string uptime;
  string line;
  long int uptime_num;

  std::ifstream filestream(kProcDirectory + kUptimeFilename);

  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);

    linestream >> uptime;
    uptime_num = stoi(uptime);
  }

  return uptime_num;
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() {
  string line, jiffy;
  long num_jiffies = 0;

  std::ifstream filestream(kProcDirectory + kStatFilename);

  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);

    while(linestream >> jiffy) {
      if (jiffy != "cpu" || jiffy != "") {
        num_jiffies += stoi(jiffy);
      }
    }
  }
  return num_jiffies;
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) {
  string line, value;
  long int total_time = 0;

  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatFilename);

  if (filestream.is_open()) {
    std::getline(filestream, line);

    std::istringstream linestream(line);
    for (int i=0; i < 17; i++) {
      linestream >> value;

      //Adding utime, stime, cutime, cstime to total_time
      if (i>=13 && i<=16) {
        total_time += stoi(value);
      }
    }
  }

  return total_time;
}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {
  string line, jiffy;
  long active_jiffies = 0;

  std::ifstream filestream(kProcDirectory + kStatFilename);

  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);

    // Using for loop to get the columns of user, nice, system, irq, softirq, steal but skips guest, guest_nice
    for (int i=0; i < 9; i++) {
      
      // Grabbing next word
      linestream >> jiffy;

      // Skips the if statement if i symbolizes the columns Idle, iowait
      if(i != 0 && i != 4 && i != 5) {
        active_jiffies += stoi(jiffy);
      }

    }
  }
  return active_jiffies;
}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
  string line, jiffy;
  long idle_jiffies = 0;

  std::ifstream filestream(kProcDirectory + kStatFilename);

  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);

    // Using for loop to get the columns of user, nice, system, irq, softirq, steal but skips guest, guest_nice
    for(int i=0; i < 9; i++) {
      // Grabbing next word
      linestream >> jiffy;

      // Skips the if statement if i symbolizes the columns Idle, iowait
      if (i == 4 || i == 5) {
        idle_jiffies += stoi(jiffy);
      }

    }
  }
  return idle_jiffies;
}

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { return {}; }

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  string line;
  string key, value;
  int total_processes;

  std::ifstream filestream(kProcDirectory + kStatFilename);

  if (filestream.is_open()) {

    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);

      linestream >> key >> value;

      if (key == "processes") {
        total_processes = stoi(value);
      }

    }
  }

  return total_processes;
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  string line;
  string key, value;
  int running_processes;

  std::ifstream filestream(kProcDirectory + kStatFilename);

  if (filestream.is_open()) {

    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);

      linestream >> key >> value;

      if (key == "procs_running") {
        running_processes = stoi(value);
      }

    }
  }

  return running_processes;
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) {
  string cmd_line;

  std::ifstream filestream(kProcDirectory + to_string(pid) + kCmdlineFilename);

  if (filestream.is_open()) {
    std::getline(filestream, cmd_line);
  }

  return cmd_line;
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) {
  string line;
  string key, value;
  int ram;

  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatusFilename);

  if (filestream.is_open()) {

    while(std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);

      linestream >> key >> value;
      if (key == "VmSize") {
        ram = stoi(value);
      }
    }
  }

  //Converting RAM from kB to MB
  ram /= 1000;

  return std::to_string(ram);
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) {
  string line;
  string UID;
  string key, value;

  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatFilename);

  if (filestream.is_open()) {
    

    while (std::getline(filestream, line)) {

      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);

      linestream >> key >> value;

      if (key == "Uid") {
        UID = value;
      }
    }
  }

  return UID;
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) {
  string uid = LinuxParser::Uid(pid);
  string line;
  string user, x, uid_val;

  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {

    while(std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> user >> x >> uid_val;
      if (uid_val == uid) {
        return user;
      }
    }
  }
  return user;
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) {
  string line, value;
  long int uptime_pid = 0;
  long int uptime = LinuxParser::UpTime();

  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    for (int i=0; i < 22; i++) {
      linestream >> value;
    }
    uptime_pid = std::stoi(value);
  }

  return uptime_pid - uptime;
}