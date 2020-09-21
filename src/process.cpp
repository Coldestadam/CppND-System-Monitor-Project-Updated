#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include <iostream>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

// TODO: Return this process's ID
int Process::Pid() { return Process::pid; }

void Process::Pid(int pid) {
    Process::pid = pid;
    Process::SetCpuUtilization();
}

// TODO: Return this process's CPU utilization
void Process::SetCpuUtilization() {
    float uptime = LinuxParser::UpTime();
    float starttime = LinuxParser::UpTime(Process::Pid());
    float total_time = LinuxParser::ActiveJiffies(Process::Pid());
    float seconds;

    seconds = uptime - (starttime/sysconf(_SC_CLK_TCK));

    Process::cpu_utilization = (total_time/sysconf(_SC_CLK_TCK))/seconds;
}

float Process::CpuUtilization() const {
    return Process::cpu_utilization;
}

// TODO: Return the command that generated this process
string Process::Command() { return LinuxParser::Command(Process::Pid()); }

// TODO: Return this process's memory utilization
string Process::Ram() { return LinuxParser::Ram(Process::Pid()); }

// TODO: Return the user (name) that generated this process
string Process::User() { return LinuxParser::User(Process::Pid()); }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::UpTime(Process::Pid())/sysconf(_SC_CLK_TCK); }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const { return a.CpuUtilization() < this->CpuUtilization(); }