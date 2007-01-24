#include <dataclasses/BoostHeaders.h>
#include "simclasses/I3MMCInfo.h"

I3MMCInfo::~I3MMCInfo(){};

template <class Archive>
void
I3MMCInfo::serialize(Archive& ar, unsigned version)
{
  ar & make_nvp("major_pid",major_pid);
  ar & make_nvp("minor_pid",minor_pid);

  ar & make_nvp("xi",xi);
  ar & make_nvp("yi",yi);
  ar & make_nvp("zi",zi);
  ar & make_nvp("ti",ti);
  ar & make_nvp("Ei",Ei);

  ar & make_nvp("xc",xc);
  ar & make_nvp("yc",yc);
  ar & make_nvp("zc",zc);
  ar & make_nvp("tc",tc);
  ar & make_nvp("Ec",Ec);

  ar & make_nvp("xf",xf);
  ar & make_nvp("yf",yf);
  ar & make_nvp("zf",zf);
  ar & make_nvp("tf",tf);
  ar & make_nvp("Ef",Ef);
};

I3_SERIALIZABLE(I3MMCInfo);
I3_SERIALIZABLE(I3MMCInfoTree);
