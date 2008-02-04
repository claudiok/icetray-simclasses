/**
 *@file
 *@brief I3SumGenerator
 *       A class for generating the sum of several random numbers which all have
 *       the same probability density, like the pulse-heights generated by several
 *       simultaneous PEs. At initialisation a set of vectors (each for a different 
 *       number of terms in the sum) is created. Each vector is a lookup table holding
 *       values of the sum which correspond to equally spaced values of the cumulative
 *       probability. This is used to generate a value for the sum very quickly from
 *       a uniformly distributed random number. To improve accuracy in the tails of 
 *       the distribution, finer tables with non-uniform spacing are used for high and 
 *       low cumulant probabilities.
 *
 *       When the number of terms exceeds a specified value the gaussian random number
 *       generator is invoked instead.
 *
 *@author Klas Hultqvist
 *(c) the IceCube Collaboration
 *
 */
#ifndef I3SUM_GENERATOR_H
#define I3SUM_GENERATOR_H
#include <vector>
#include "phys-services/I3RandomService.h"
  class I3SumGenerator {
 public:
  /**
    *@brief  Construct an uninitialised SumGenerator. Will not work before it's 
    * initialised.
    */
  I3SumGenerator(){};
  /**
   *@brief Construct a SumGenerator
   *The parameters are the same as for Initialise()
   */
  I3SumGenerator(I3RandomServicePtr r,double (*fun)(double),
			       const double &xlo, const double &xhi, const int &nbins, 
			       const int &switchgauss, 
			       const double &PLow=0.001, const int &nBinsLow=100, 
			       const double &PHigh=0.999, const int &nBinsHigh=100);
  /**
   *@brief Generate the value of a sum of random numbers 
   *@param terms   Number of terms in the sum
   */
  double Generate(int terms);
  /**
   *@brief      Initialise a SumGenerator
   *@param r            Pointer to random number service
   *@param fun          Pointer to probability density function
   *@param xlo          Low edge of interval where fun is defined
   *@param xhi          High edge
   *@param nbins        Number of bins to calculate fun in and to use in lookup table
   *@param switchGauss  The minimum number of terms for which gaussian generation
   *                    will be enabled 
   *@param PLow         The probability defining the low tail
   *@param nBinsLow     The number of bins (samples) used for the low tail
   *@param PHigh        The probability defining the high tail 
   *@param nBinsHigh    The number of bins (samples) used for the high tail
   */
  void Initialise(I3RandomServicePtr r,double (*fun)(double),
			       const double &xlo, const double &xhi, const int &nbins, 
			       const int &switchgauss, 
			       const double &PLow=0.001, const int &nBinsLow=100, 
			       const double &PHigh=0.999, const int &nBinsHigh=100);
 private:
  /**
   * Lowest number of terms for which gaussian generation is used 
   */
  int switchGauss_;
  /**
   * Number of bins (same in sum value and probability)
   */
  int nBins_;
  /**
   * Min and max of single term
   */
  double xLo_,xHi_;
  /**
   * Expectation value and standard deviation for single term
   */
  double expectVal_,stdDev_;
  /**
   * Vector of sum values (lookup table) for different cumulative probabilities
   */
  vector< vector<double> > X_;
  /**
   * Number of bins in finer lookup tables for use in high and low tails
   */
  int nBinsLow_,nBinsHigh_;
  /**
   * Probabilities corresponding to change of lookup table
   */
  double PLow_,PHigh_;
  /**
   * Stepsizes for cubic root of probability difference in low and high tail 
   */
  double binStepLow_,binStepHigh_;
  /**
   * Lookup tables for high and low tails
   */
  vector< vector<double> > XLow_,XHigh_;
  /**
   * Pointer to random number generator service
   */
  I3RandomServicePtr random_;
};
#endif