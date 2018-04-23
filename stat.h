#ifndef STAT_H
#define STAT_H
#include "stdio.h"
#include "math.h"
class Stat 
{
 public:
  Stat(); 
  double* data; 
  int start, end, size;
  double Mean, Stdev, AutoCorrelation, Error; 
  Stat(int size, int a, int b);
  double GetMean(); 
  double GetStdev(); 
  double GetError(); 
  double GetAutoCorrelation(); 
  double GetCt(int t); 
  void Analyze();  
  void Print(); 
}; 
#endif
