#include <icetray/serialization.h>
#include "simclasses/I3Histogram.h"



I3Histogram::I3Histogram() :
  nbin_(0),
  minbin_(NAN),
  maxbin_(NAN),
  init_(false) { }

I3Histogram::I3Histogram(int nbin, double minbin,double maxbin) :
  nbin_(nbin),
  minbin_(minbin),
  maxbin_(maxbin),
  init_(true)
{ 
	if (minbin_>maxbin_) 
		log_fatal("histogram has lowerbound > uppebound!!!.");
	if (!nbin_) 
		log_fatal("histogram has zero size!!.");
  	hist_ = vector<int>(nbin_);
}

void
I3Histogram::SetMinBin(double minbin)
{
	if (init_) { 
		log_fatal("Histogram has already been initialized.");
	} else {
  		minbin_ = minbin;
  		if (nbin_ && !isnan(maxbin_)) {
			init_ = true;
			hist_ = vector<int>(nbin_);
		}
  		if (minbin_>maxbin_) 
			log_fatal("histogram has lowerbound > uppebound!!!.");
	}
}

void
I3Histogram::SetMaxBin(double maxbin)
{
	if (init_) { 
		log_fatal("Histogram has already been initialized.");
	} else {
  		maxbin_ = maxbin;
  		if (nbin_ && !isnan(minbin_)) {
			init_ = true;
			hist_ = vector<int>(nbin_);
		}
  		if (minbin_>maxbin_) 
			log_fatal("histogram has lowerbound > uppebound!!!.");
	}
}

void
I3Histogram::SetSize(unsigned bins)
{
	if (!bins) { 
		log_fatal("Cannot set number of bins to zero");
	}
	if (init_) { 
		log_fatal("Histogram has already been initialized.Cannot set number of bins");
	} else {
  		nbin_=bins;
  		if (!isnan(maxbin_) && !isnan(minbin_)) {
			init_ = true;
			hist_ = vector<int>(nbin_);
		}
	}
}

double I3Histogram::GetBinSize() const
{
	if (!init_) { 
		log_fatal("Histogram has already been initialized.Cannot set number of bins");
	}
	return (maxbin_-minbin_)/hist_.size();
}

double I3Histogram::GetBinValue(unsigned bin) const
{
	if (!init_) { 
		log_fatal("Histogram has already been initialized.Cannot set number of bins");
	}
	return minbin_+bin*GetBinSize();
}

double I3Histogram::GetMean() const
{
	if (!init_) { 
		log_fatal("Histogram has already been initialized.Cannot set number of bins");
	}
	double binsize = GetBinSize();
	double sum =0.;
	double isum =0;

	for (unsigned index=0;index < GetSize();++index)
    {
		sum += hist_[index]*(index*binsize+minbin_);
		isum += hist_[index];
    }
	return sum/isum;
}

int& I3Histogram::operator[](double value){
	if (!init_) { 
		log_fatal("Histogram not been initialized.");
	}
	int index = int((value-minbin_)/GetBinSize());
	unsigned uindex = max(0,index);
	uindex = min(uindex,unsigned(GetSize()-1));
	return hist_[uindex];
}

template <class Archive> 
void I3Histogram::serialize(Archive& ar, unsigned version)
{
  ar & make_nvp("NumberOfBins", nbin_);
  ar & make_nvp("MinBin", minbin_);
  ar & make_nvp("MaxBin", maxbin_);
  ar & make_nvp("init", init_);
  ar & make_nvp("vector<int>", hist_);
}
                                                                                                          
I3_SERIALIZABLE(I3Histogram);
