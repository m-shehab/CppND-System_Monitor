#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  float Utilization();  

  
 private:
  
  //For calculating CPU utilization depending on prev measurements 
  float previdle=0.0, previowait=0.0, prevuser=0.0, prevnice=0.0;
  float prevsystem=0.0, previrq=0.0, prevsoftirq=0.0, prevsteal=0.0;
};

#endif