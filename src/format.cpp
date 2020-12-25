#include <string>
#include <iomanip>
#include "format.h"

using std::string;
using std::to_string;

// DONE: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string Format::ElapsedTime(long seconds) { 
  int HH,MM,SS;
  string H,M,S;
  int S_in_H = 60 * 60; 
  int M_in_H = 60; 
  int S_in_M = 60;
  
  HH = seconds / S_in_H;
  H = to_string(HH);
  if(HH < 10){H = "0"+ H;}
  
  seconds = seconds % S_in_H;
  MM = seconds / M_in_H;
  M = to_string(MM);
  if(MM < 10){M = "0"+ M;}
  
  seconds = seconds % S_in_M;
  SS = seconds % S_in_M;
  S = to_string(SS);
  if(SS < 10){S = "0"+ S;}
  
  return H+":"+M+":"+S; 
}