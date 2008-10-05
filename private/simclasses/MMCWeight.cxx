#include <icetray/serialization.h>
#include "simclasses/MMCWeight.h"

template <class Archive>
void MMCWeight::serialize(Archive& ar, unsigned version)
{

  ar & make_nvp("I3FrameObject",     base_object<I3FrameObject>(*this));
  ar & make_nvp("weight",            weight );
  ar & make_nvp("distToModIntPoint", distToModIntPoint );

}

BOOST_CLASS_VERSION(MMCWeight, 1);
I3_SERIALIZABLE(MMCWeight);
