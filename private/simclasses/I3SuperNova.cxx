#include <icetray/serialization.h>
#include "simclasses/I3SuperNova.h"

template <class Archive> 
void I3SuperNova::serialize(Archive& ar, unsigned version)
{
  ar & make_nvp("NumberOfBins", NumberOfBins_);
  ar & make_nvp("NumberOfNeutrinos", NumberOfNeutrinos_);
  ar & make_nvp("NumberOfBeta", NumberOfBeta_);
  ar & make_nvp("NumberOfPhotons", NumberOfPhotons_);
  ar & make_nvp("NumberOfPEs", NumberOfPEs_);
  ar & make_nvp("I3Map<OMKey,unsigned int>", DOMCount_);

  ar & make_nvp("DeltaT", DeltaT_);
  ar & make_nvp("Time", Time_);
  ar & make_nvp("SNdistance", SNdistance_);
  ar & make_nvp("SNdirection", SNdirection_);
  ar & make_nvp("I3Map<double,double>", EnergyHistogram_);
  ar & make_nvp("NuFlavor", NuFlavor_);
}
                                                                                                          
I3_SERIALIZABLE(I3SuperNova);

I3_SERIALIZABLE(I3SuperNovaTree);
