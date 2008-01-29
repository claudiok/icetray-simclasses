#include <iostream>
#include <cmath>
using namespace std;
#include <string>
using std::string;
#include "phys-services/I3RandomService.h"
#include "simclasses/I3SumGenerator.h"
/**
 * Constructor with arguments, just calls Initialise method
 */
I3SumGenerator::I3SumGenerator(I3RandomServicePtr r,double (*fun)(double),double xlo, double xhi, int nbins, int switchgauss)
{
  Initialise(r,fun,xlo,xhi,nbins,switchgauss);
}
/**
 * Initialise I3SumGenerator
 */
void I3SumGenerator::Initialise(I3RandomServicePtr r,double (*fun)(double),double xlo, double xhi, int nbins, int switchgauss)
{
  int bin,bin1;
  vector<double>total;
  vector< vector<double> > P;

  /**
   * Initialise class variables
   */
  random_ = r;
  xLo_ = xlo;
  xHi_ = xhi;
  switchGauss_ = switchgauss;
  nBins_ = nbins;
  double dx( (xhi - xlo) / nbins );

  /** 
   * Sum (integral) for different number of terms
   */
  total.resize(switchgauss);
  /**
   * Vectors holding probabilities for different values of sum 
   */
  P.resize(switchgauss);
  /**
   * Unnormalised integral (sum over sampling points, really) for single term
   */
  double x( xlo+0.5*dx );
  double sumvalue(0),fvalue;
  for(bin=1; bin <= nbins ; bin++) {
    fvalue = fun(x);
    sumvalue += fvalue;
    x+=dx;
  }

  /**
   * Normalise probabilities, and calculate mean and sigma for single term
   */
  expectVal_=0;
  double sumsq(0);
  x = xlo+0.5*dx;
  P[1].resize(nbins+1);
  P[1][0]=0;
  for(bin=1; bin <= nbins ; bin++) {
    fvalue = fun(x) / sumvalue;
    expectVal_ += x*fvalue;
    sumsq += x*x*fvalue;
    P[1][bin] = fvalue;
    x+=dx;
  }
  stdDev_ = sqrt(sumsq - expectVal_*expectVal_);
  // We normalised to unity:
  total[1] = 1;

  /**
   * Loop over number of terms from 2 up, recursively filling the vectors of 
   * sampled probability densities. The number of points is the same for different
   * number of terms, so the spacing is equal to the number of terms.
   */
  for(int terms=2; terms<switchgauss; terms++) 
    {
      P[terms].resize(nbins+1);
      total[terms] = 0;
      P[terms][0]=0;
      int bin,binold;
      // Loop over bins (samples) for current number of terms
      for(bin=0; bin <= nbins ; bin++) {
        P[terms][bin] = 0;
	// For each bin, loop over bins (samples) in terms-1 vector, stopping when it
	// becomes too big for the wanted value of bin
	for(binold=0; binold <= nbins && (terms-1)*binold <= terms*bin ;binold++) {
	  // Work out the bin (sample) number in the single term vector corresponding
	  // to the added term, and check that a solution exists
	  bin1 = terms*bin - (terms-1)*binold;
          if(bin1 <= nbins) 
	    {
	      // Add the probabilities contributing to bin
	      P[terms][bin] += P[terms-1][binold]*P[1][bin1];
	    }
	}
	// Sum the probabilities for terms
	total[terms] += P[terms][bin];
      }
    }

  /**
   * Now calculate the normalised cumulants and invert to get a quick mapping from
   * cumulative probability to value of sum (X_)
   */
  X_.resize(switchgauss);  
  for(int terms=1; terms<switchgauss; terms++) {
    // Convert to cumulant probabilities
    for(bin=1; bin <= nbins ; bin++) P[terms][bin] = P[terms][bin]/total[terms] + P[terms][bin-1] ;
    // Make sure to avoid round-off errors in probability sum
    P[terms][nbins] = 1;
    //Fill the X_-vector for this number of terms
    X_[terms].resize(nbins+2);
    //Start with the first x bin (x is the sum of terms)
    int binxBefore(0),binxAfter(1);
    double prob,dprob(1/(double)nbins);
    /**
    * Loop over probability bins (sample points) and move the x bin along until 
    * it matches current probability
    */
    for(int binP=0;binP<nbins;binP++) {
      prob=dprob*binP;
      while (P[terms][binxAfter] < prob) binxAfter++;
      binxBefore=binxAfter-1;
      // Find x value for current probability value by interpolation
      X_[terms][binP] = terms*dx*( binxBefore + (prob-P[terms][binxBefore])/
				   (P[terms][binxAfter] - P[terms][binxBefore]) );
    }
    X_[terms][nbins] = terms*xhi;
    // Add an extra element just in case uniform random number sometimes equals one
    // in Generate()
    X_[terms][nbins+1] = X_[terms][nbins];
  }
}
 
double I3SumGenerator::Generate(int terms)
{
  double retval;
  if(terms < switchGauss_){
    /**
     * Low number of terms, read off sum from X_ vector for a random
     * value of cumulative probability
     */
    double xi = nBins_*random_->Uniform(1.);
    int bin = static_cast<int>(xi);
    double binfrac = xi - bin;
    retval = X_[terms][bin] + binfrac*(X_[terms][bin+1] - X_[terms][bin]);
  } else {
    /**
     * Large number of terms, rely on central limit theorem
     */
    double sigma = stdDev_*sqrt((double)terms);
    retval = random_->Gaus(expectVal_*terms ,sigma);
    while(retval<terms*xLo_ || retval>terms*xHi_)
    retval = random_->Gaus(expectVal_*terms ,sigma);
  }
  return retval;
}
