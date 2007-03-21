#include <dataclasses/BoostHeaders.h>
#include "mmc-icetray/I3MMCTrack.h"

template <class Archive> 
void I3MMCTrack::serialize(Archive& ar, unsigned version)
{
  ar & make_nvp("I3Particle", particle_);
  ar & make_nvp("xi", xi);
  ar & make_nvp("yi", yi);
  ar & make_nvp("zi", zi);
  ar & make_nvp("ti", ti);
  ar & make_nvp("Ei", Ei);
  ar & make_nvp("xf", xf);
  ar & make_nvp("yf", yf);
  ar & make_nvp("zf", zf);
  ar & make_nvp("tf", tf);
  ar & make_nvp("Ef", Ef);
  ar & make_nvp("xc", xc);
  ar & make_nvp("yc", yc);
  ar & make_nvp("zc", zc);
  ar & make_nvp("tc", tc);
  ar & make_nvp("Ec", Ec);
  ar & make_nvp("Elost", Elost);
}
                                                                                                          
I3_SERIALIZABLE(I3MMCTrack);

I3_SERIALIZABLE(MMCTrackList);
