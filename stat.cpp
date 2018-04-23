#include "stat.h"
#include "stdio.h"
#include "iostream"
using namespace std; 
void Stat::Analyze() {
    GetAutoCorrelation();
    GetError(); 
    Print(); 
} 
void Stat::Print() {
    printf("\n    Result of statistics \n-------------------\n# of data: %d\n", end - start); 
    printf("     Mean: %lf\n", Mean); 
    printf("    Stdev: %lf\n", Stdev);
    printf("    Error: %lf\n", Error); 
    printf(" AutoCorr: %lf\n", AutoCorrelation);
    printf("--------------------\n     End of statistics\n"); 
}

Stat::Stat(int size, int a, int b) 
{
  data = new double[size]; 
  start=a; 
  end=b; 
  Mean = 0.0; 
  Stdev = 0.0; 
  Error = 0.0; 
}
double Stat::GetMean()
{
  double mean=0.0; 
  for(int i=start; i<end; i++) {
    mean += data[i]; 
  }
  Mean = mean/(end - start); 
  return Mean; 
}

double Stat::GetStdev()
{
  double x=0.0; 
  double xx = 0.0; 
  for(int i=start; i<end; i++) {
    x += data[i]; 
    xx += data[i]*data[i]; 
  }
  double mu = x/(end - start); 
  Stdev =  sqrt(xx/(end - start) - mu*mu);  
  return Stdev; 
}

double Stat::GetError()
{
  GetStdev(); 
  if (end - start > 1) { 
    Error = Stdev/sqrt(end - start - 1); 
    return Error; 
  }
  else {
    Error = Stdev; 
    return Stdev; 
  }
}


double Stat::GetAutoCorrelation()
{
  int t = 1; 
  double ac = 0.0; 
  while( GetCt(t) > 0 && t < end - start) {
    ac += GetCt(t); 
    t++; 
  }
  AutoCorrelation = 1 + 2*ac; 
  return AutoCorrelation; 
}

double Stat::GetCt(int t)
{
  GetMean(); 
  GetStdev();
  double temp = 0.0; 
  for(int i=start; i<end - t; i++) {
    temp += (data[i] - Mean)*(data[t+i] - Mean); 
  }
  return temp/Stdev/Stdev/(end - start - t); 
}
