#ifndef I3MCPULSE_H_INCLUDED
#define I3MCPULSE_H_INCLUDED

#include <algorithm>
#include <vector>
#include <boost/foreach.hpp>
#include <icetray/I3Logging.h>
#include <icetray/serialization.h>
#include <dataclasses/I3Map.h>
#include <dataclasses/ParticleIDKey.h>

static const unsigned i3mcpulse_version_ = 0;

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
