#ifndef I3MCSPE_H_INCLUDED
#define I3MCSPE_H_INCLUDED

#include <algorithm>
#include <vector>
#include <boost/foreach.hpp>
#include <icetray/I3Logging.h>
#include <icetray/serialization.h>
#include <dataclasses/I3Map.h>

static const unsigned i3mcspe_version_ = 0;

struct I3MCSPE {

public:

  float time;
  uint32_t npe;
  int64_t major_ID;
  uint32_t minor_ID;

  SET_LOGGER("I3MCSPE");

  bool operator==(const I3MCSPE& rhs) {
    return time == rhs.time
      && npe == rhs.npe
      && major_ID == rhs.major_ID
      && minor_ID == rhs.minor_ID;
  }
  
private:
  friend class boost::serialization::access;
  template <class Archive> void serialize(Archive & ar, const unsigned version)
  {
    ar & make_nvp("time",time);
    ar & make_nvp("npe",npe);
    ar & make_nvp("major_ID",major_ID);
    ar & make_nvp("minor_ID",minor_ID);
  }

};


BOOST_CLASS_VERSION(I3MCSPE,i3mcspe_version_);
typedef std::vector<I3MCSPE> I3MCSPESeries;
typedef I3Map<OMKey, I3MCSPESeries > I3MCSPESeriesMap;
I3_POINTER_TYPEDEFS(I3MCSPESeries);
I3_POINTER_TYPEDEFS(I3MCSPESeriesMap);

#endif
