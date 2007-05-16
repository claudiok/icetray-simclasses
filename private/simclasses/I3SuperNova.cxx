#include <icetray/serialization.h>
#include "simclasses/I3SuperNova.h"

int I3SuperNova::global_minor_id_ = 0;

I3SuperNova::I3SuperNova() :
  parentID_(-1),
  NuFlavor_(I3Particle::NuE) // default
{
  	ID_ = global_minor_id_++;
}
I3SuperNova::I3SuperNova(I3Particle::ParticleType flavor) :
  parentID_(-1),
  NuFlavor_(flavor)
{ 
  	ID_ = global_minor_id_++;
}
template <class Archive> 
void I3SuperNova::serialize(Archive& ar, unsigned version)
{
  ar & make_nvp("NumberOfNeutrinos", NumberOfNeutrinos_);
  ar & make_nvp("NumberOfSecondaries", NumberOfSecondaries_);
  ar & make_nvp("NumberOfPhotons", NumberOfPhotons_);
  ar & make_nvp("NumberOfPEs", NumberOfPEs_);
  ar & make_nvp("I3Map<OMKey,unsigned int>", DOMCount_);

  ar & make_nvp("InteractionWeight",InteractionWeight_);
  ar & make_nvp("SNdistance", SNdistance_);
  ar & make_nvp("SNdirection", SNdirection_);
  ar & make_nvp("I3Map<double,double>", EnergyHistogram_);
  ar & make_nvp("NuFlavor", NuFlavor_);
}
                                                                                                          
I3_SERIALIZABLE(I3SuperNova);

I3_SERIALIZABLE(I3SuperNovaTree);
