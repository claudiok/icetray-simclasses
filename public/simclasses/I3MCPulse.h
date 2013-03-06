#ifndef I3MCPULSE_H_INCLUDED
#define I3MCPULSE_H_INCLUDED

#include <algorithm>
#include <vector>
#include <boost/foreach.hpp>
#include <icetray/I3Logging.h>
#include <icetray/serialization.h>
#include <dataclasses/I3Map.h>
#include <simclasses/ParticleIDKey.h>

static const unsigned i3mcpulse_version_ = 0;
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
typedef std::vector<I3MCSPE> I3MCSPEVector;
typedef I3Map<OMKey, I3MCSPEVector > I3MCSPEVectorMap;
I3_POINTER_TYPEDEFS(I3MCSPEVector);
I3_POINTER_TYPEDEFS(I3MCSPEVectorMap);


struct I3MCPulse {

public:
  enum PulseSource{
    UNKNOWN = 0,
    SPE = 10,
    RANDOM = 20,
    AFTER_PULSE = 30,
    PRE_PULSE = 40,
    ELASTIC_LATE_PULSE = 50,
    INELASTIC_LATE_PULSE = 60,
    EARLY_AFTER_PULSE = 70
  };

  float time;
  float charge;
  PulseSource source;

  SET_LOGGER("I3MCPulse");

  bool operator==(const I3MCPulse& rhs) {
    return time == rhs.time
      && charge == rhs.charge
      && source == rhs.source;
  }

  I3MCPulse(float t, float c = 1.0 , PulseSource s = SPE) :
    time(t), charge(c), source(s) {};

  I3MCPulse(){};  
  
private:
  friend class boost::serialization::access;
  template <class Archive> void serialize(Archive & ar, const unsigned version)
  {
    ar & make_nvp("time",time);
    ar & make_nvp("charge",charge);
    ar & make_nvp("source",source);
  }

};

BOOST_CLASS_VERSION(I3MCPulse,i3mcpulse_version_);

typedef std::vector<I3MCPulse> I3MCPulseSeries;
typedef I3Map<OMKey, I3MCPulseSeries > I3MCPulseSeriesMap;
I3_POINTER_TYPEDEFS(I3MCPulseSeries);
I3_POINTER_TYPEDEFS(I3MCPulseSeriesMap);

#endif
