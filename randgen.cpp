#include "gsl/gsl_rng.h"
#include "gsl/gsl_randist.h"
#include "randgen.h"
const gsl_rng_type * T = gsl_rng_default;
const gsl_rng * r = gsl_rng_alloc (T);
double Rng::Gaussian(double sigma)
{
  return gsl_ran_gaussian (r, sigma);
}

double Rng::Uniform(void)
{
  return gsl_rng_uniform(r);
}

double Rng::Rint(int n)
{
  return int(Uniform()*n); 
}
