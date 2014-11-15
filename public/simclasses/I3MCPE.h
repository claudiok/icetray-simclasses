/**
 * copyright  (C) 2013
 * the icecube collaboration
 * @version $Id: $
 */

#ifndef I3MCPE_H_INCLUDED
#define I3MCPE_H_INCLUDED

#include <algorithm>
#include <vector>
#include <boost/foreach.hpp>
#include <icetray/I3Logging.h>
#include <icetray/serialization.h>
#include <dataclasses/I3Map.h>
#include <dataclasses/physics/I3Particle.h>

class I3Particle;

static const unsigned i3mcpe_version_ = 1;

/**
 * @brief I3MCPE struct that stores the photon arrival time 
 * (i.e.PE creation time), number of PE (for binning), and
 * the IDs of the particle that created this.
 */

struct I3MCPE {

  /** 
   * IDs of the I3Particle that created this PE
   */   
  uint64_t major_ID;
  int32_t minor_ID;

  /**
   * Creation time of PE (photon arrival time)
   */ 
  double time;

  /**
   * Number of PEs this object represents.
   * Used for binning.
   */
  uint32_t npe;

  SET_LOGGER("I3MCPE");

  bool operator==(const I3MCPE& rhs) const {
    return time == rhs.time
    && npe == rhs.npe
    && major_ID == rhs.major_ID
    && minor_ID == rhs.minor_ID;
  }
	
  // default constructor for noise generators
  I3MCPE():major_ID(0),minor_ID(0),npe(0){}

  // constructor for hit makers
  // this just sets the major and minor IDs accordingly
  I3MCPE(const I3Particle& p):
  major_ID(p.GetMajorID()),minor_ID(p.GetMinorID()),npe(0){}

  I3MCPE(uint64_t major_ID, int32_t minor_ID):
  major_ID(major_ID),minor_ID(minor_ID),npe(0){}
  
private:
  friend class boost::serialization::access;
  template <class Archive> void serialize(Archive & ar, const unsigned version)
  {
    if (version>i3mcpe_version_)
      log_fatal("Attempting to read version %u from file but running version %u of I3MCPE class.",
                version,i3mcpe_version_);
    if(version == 0){
      float t(0.);
      ar & make_nvp("time",t);
      time = t;
    }else{
      ar & make_nvp("time",time);
    }
    ar & make_nvp("npe",npe);
    ar & make_nvp("major_ID",major_ID);
    ar & make_nvp("minor_ID",minor_ID);
  }

};

BOOST_CLASS_VERSION(I3MCPE,i3mcpe_version_);
typedef std::vector<I3MCPE> I3MCPESeries;
typedef I3Map<OMKey, I3MCPESeries > I3MCPESeriesMap;
I3_POINTER_TYPEDEFS(I3MCPE);
I3_POINTER_TYPEDEFS(I3MCPESeries);
I3_POINTER_TYPEDEFS(I3MCPESeriesMap);

#endif
