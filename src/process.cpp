#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;
using namespace LinuxParser;

Process::Process(int P){
	pid = P;
}

// DONE: Return this process's ID
int Process::Pid() { return pid; }

// DONE: Return this process's CPU utilization
float Process::CpuUtilization() { 
  float p_age = (float)UpTime();
  float p_active = (float)(LinuxParser::ActiveJiffies(pid)/sysconf(_SC_CLK_TCK));
  return (p_active/p_age)<1.0?(p_active/p_age):1; 
}

// DONE: Return the command that generated this process
string Process::Command() { return LinuxParser::Command(pid); }

// DONE: Return this process's memory utilization
string Process::Ram() { return LinuxParser::Ram(pid); }

// DONE: Return the user (name) that generated this process
string Process::User() { return LinuxParser::User(pid); }

// DONE: Return the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::UpTime() - LinuxParser::UpTime(pid); }

// DONE: Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process& a) { return (this->CpuUtilization() < a.CpuUtilization()); }