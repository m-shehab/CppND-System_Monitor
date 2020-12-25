#include <vector>
#include <string>
#include "processor.h"
#include "linux_parser.h"

using std::vector;
using std::string;
using std::stof;

using namespace LinuxParser;
  
float Processor::Utilization() { 
  vector<string> CPU_Ts = LinuxParser::CpuUtilization();
  
  float user, nice, system, idle, iowait, irq, softirq, steal;
  user = stof(CPU_Ts[1]);
  nice = stof(CPU_Ts[2]);
  system = stof(CPU_Ts[3]);
  idle = stof(CPU_Ts[4]);
  iowait = stof(CPU_Ts[5]);
  irq = stof(CPU_Ts[6]);
  softirq = stof(CPU_Ts[7]);
  steal = stof(CPU_Ts[8]);
  float PrevIdle = previdle + previowait;
  float Idle = idle + iowait;
  float PrevNonIdle = prevuser + prevnice + prevsystem + previrq + prevsoftirq + prevsteal;
  float NonIdle = user + nice + system + irq + softirq + steal;
  float PrevTotal = PrevIdle + PrevNonIdle;
  float Total = Idle + NonIdle;
  float totald = Total - PrevTotal;
  float idled = Idle - PrevIdle;
  float cpu_util = (totald - idled)/totald;
  prevuser = user;
  prevnice = nice;
  prevsystem = system;
  previdle = idle;
  previowait = iowait;
  previrq = irq;
  prevsoftirq = softirq; 
  prevsteal = steal; 
  return cpu_util; 
}