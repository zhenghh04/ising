#ifndef Rng_h
#define Rng_h
#include <gsl/gsl_rng.h>
extern const gsl_rng_type * T; 
extern const gsl_rng * r; 
class Rng
{
 public:
  Rng(int a=0) {
    set(a); 
  }; 
  double Gaussian(double sigma); 
  double Uniform(); 
  double Rint(int); 
  void set(int a) {
    gsl_rng_set(r, a); 
  }; 
}; 
#endif
