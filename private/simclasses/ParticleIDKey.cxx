#include <icetray/serialization.h>
#include <simclasses/ParticleIDKey.h>
#include <dataclasses/physics/I3Particle.h>

ParticleIDKey::ParticleIDKey(const I3Particle& p) :
  minor_ID(p.GetMinorID()),
  major_ID(p.GetMajorID())
{};

template <class Archive> 
void ParticleIDKey::serialize(Archive & ar, unsigned version){
  ar & make_nvp("MinorID",minor_ID);
  ar & make_nvp("MajorID", major_ID);
}

I3_SERIALIZABLE(ParticleIDKey);
