#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <unistd.h>

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

// DONE: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
  string mem, amount, unit; 
  string line;
  float MemAmount[2], MemUtil;
  
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if(stream.is_open()){
    int i = 0;
    while(std::getline(stream, line)){
      std::istringstream linestream(line);
      while(linestream >> mem >> amount >> unit){
        MemAmount[i] = (float) stoi(amount);
        i++;
        if(i == 2){
          MemUtil = (MemAmount[0]-MemAmount[1])/MemAmount[0];
          return MemUtil;
        }
      }
    } 
  }
  return 0.0;
}

// DONE: Read and return the system uptime
long LinuxParser::UpTime() {
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if(stream.is_open()){
    std::getline(stream,line,' ');
    return stoi(line);
  }
  return 0; 
}

// DONE: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { 
  vector<string> Js = CpuUtilization();
  long total_Jiffies;
  if(Js.size()>0){
    for(int i = 1; i < 9;i++){
      total_Jiffies += stoi(Js[i]);
    }
    return total_Jiffies;
  }
  return 0; 
}

// DONE: Read and return the number of active jiffies for a PID
long LinuxParser::ActiveJiffies(int pid) { 
  string p = to_string(pid);
  string line, num;
  std::ifstream stream(kProcDirectory + p + kStatFilename);
  if(stream.is_open()){
    std::getline(stream,line);
    std::stringstream linestream(line);
    long total_jiffies;
    int i = 0;
    while(std::getline(linestream, num, ' ')){
      i++;
      if(i==14||i==15||i==16){
        total_jiffies += stoi(num);
      }
      if(i==17){
        total_jiffies += stoi(num);
        return total_jiffies;
      }
    }
  }  
  return 0; 
}

// DONE: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { 
  return Jiffies()-IdleJiffies(); 
}

// DONE: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { 
  string line, temp;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if(stream.is_open()){
    std::getline(stream,line);
    std::istringstream linestream(line);
    int i = 0;
    while(linestream >> temp){
      i++;
      if(i==5){
        return stoi(temp);
      }
    }
  }
  return 0; 
}

// DONE: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { 
  vector<string> CPU_Ts;
  string line, temp;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if(stream.is_open()){
    std::getline(stream,line);
    std::istringstream linestream(line);
    int i = 0;
    while(linestream >> temp){
      i++;
      CPU_Ts.push_back(temp);
      if(i==9){
        return CPU_Ts;
      }
    }
  }
  return {}; 
}

// DONE: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  string key, value, line;
  int num; 
  std::ifstream stream(kProcDirectory + kStatFilename);
  if(stream.is_open()){
  	while(std::getline(stream, line)){
      std::istringstream linestream(line);;
      while(linestream >> key >> value){
      	if(key == "processes"){
          return stoi(value); 
        }
      }
    }
  }
  return 0; 
}

// DONE: Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  string key, value, line;
  int num; 
  std::ifstream stream(kProcDirectory + kStatFilename);
  if(stream.is_open()){
  	while(std::getline(stream, line)){
      std::istringstream linestream(line);;
      while(linestream >> key >> value){
      	if(key == "procs_running"){
          return stoi(value); 
        }
      }
    }
  }
  return 0;  
}

// DONE: Read and return the command associated with a process
string LinuxParser::Command(int pid) { 
  string p = to_string(pid);
  string line;
  std::ifstream stream(kProcDirectory + p + kCmdlineFilename);
  if(stream.is_open()){
  	std::getline(stream,line);
    return line;
  }
  return string(); 
}

// DONE: Read and return the memory used by a process
string LinuxParser::Ram(int pid) { 
  string p = to_string(pid);
  string Mem, amount, unit;
  string line;
  std::ifstream stream(kProcDirectory + p + kStatusFilename);
  if(stream.is_open()){
  	while(getline(stream,line)){
      std::istringstream linestream(line);
      while(linestream >> Mem >> amount >> unit){
      	if(Mem == "VmSize:"){
          return amount;
        }
      }
    }
  }
  return string(); 
}

// DONE: Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) { 
  string p = to_string(pid);
  string line, User, ID;
  std::ifstream stream(kProcDirectory + p + kStatusFilename);
  if(stream.is_open()){
  	while(getline(stream,line)){
      std::istringstream linestream(line);
      while(linestream >> User >> ID){
        if(User == "Uid:"){
          return ID;
        }
      }
    }
  }
  return string(); 
}

// DONE: Read and return the user associated with a process
string LinuxParser::User(int pid) { 
  string ID = LinuxParser::Uid(pid);
  string line, User, Num, Sym;
  std::ifstream stream(kPasswordPath);
  if(stream.is_open()){
    while(std::getline(stream,line)){
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while(linestream >> User >> Sym >> Num){
        if(Num == ID){
          return User;
        }
      }
    }
  }
  return string(); 
}

// DONE: Read and return the uptime of a process
long LinuxParser::UpTime(int pid) { 
  string p = to_string(pid);
  string line, num;
  std::ifstream stream(kProcDirectory + p + kStatFilename);
  if(stream.is_open()){
    std::getline(stream,line);
    std::stringstream linestream(line);
    int i = 0;
    while(std::getline(linestream, num, ' ')){
      i++;
      if(i==22){
        return stoi(num)/sysconf(_SC_CLK_TCK);
      }
    }
  }
  return 0; 
}