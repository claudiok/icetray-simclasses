/**
 * copyright  (C) 2013
 * the icecube collaboration
 * @version $Id: $
 */

#ifndef I3MCPULSE_H_INCLUDED
#define I3MCPULSE_H_INCLUDED

#include <algorithm>
#include <iterator>
#include <vector>
#include <boost/foreach.hpp>
#include <icetray/I3Logging.h>
#include <icetray/serialization.h>
#include <dataclasses/I3Map.h>

static const unsigned i3mcpulse_version_ = 1;

/**
 * @brief I3MCPulse struct that stores the time, charge,
 * and source of a PMT pulse.
 */

#define I3MCPULSE_H_I3MCPulse_PulseSource (UNKNOWN)(PE)(RANDOM)(AFTER_PULSE)(PRE_PULSE)(ELASTIC_LATE_PULSE)(INELASTIC_LATE_PULSE)(EARLY_AFTER_PULSE)

struct I3MCPulse {

  enum PulseSource{
    UNKNOWN = 0,
    PE = 10,
    RANDOM = 20,
    AFTER_PULSE = 30,
    PRE_PULSE = 40,
    ELASTIC_LATE_PULSE = 50,
    INELASTIC_LATE_PULSE = 60,
    EARLY_AFTER_PULSE = 70
  };

  double time;
  float charge;
  PulseSource source;

  SET_LOGGER("I3MCPulse");

  bool operator==(const I3MCPulse& rhs) const {
    return time == rhs.time
    && charge == rhs.charge
    && source == rhs.source;
  }

  I3MCPulse(float t, float c = 1.0 , PulseSource s = PE) :
    time(t), charge(c), source(s) {};

  I3MCPulse(){};  
  
private:
  friend class boost::serialization::access;
  template <class Archive> void serialize(Archive & ar, const unsigned version)
  {
    if(version == 0){
      float t(0.);
      ar & make_nvp("time",t);
      time = t;
    }else{
      ar & make_nvp("time",time);
    }
    ar & make_nvp("charge",charge);
    ar & make_nvp("source",source);
  }

};

BOOST_CLASS_VERSION(I3MCPulse,i3mcpulse_version_);

typedef std::vector<I3MCPulse> I3MCPulseSeries;
typedef I3Map<OMKey, I3MCPulseSeries > I3MCPulseSeriesMap;
I3_POINTER_TYPEDEFS(I3MCPulseSeries);
I3_POINTER_TYPEDEFS(I3MCPulseSeriesMap);

namespace detail{

	class AugmentedI3MCPulse : public I3MCPulse{
	public:
		uint32_t index;
		AugmentedI3MCPulse(const I3MCPulse& p, uint32_t i):I3MCPulse(p),index(i){}
	};

	struct PulseAugmenter{
	private:
		uint32_t index;
	public:
		PulseAugmenter():index(0){}
		
		AugmentedI3MCPulse operator()(const I3MCPulse& p){
			return(AugmentedI3MCPulse(p,index++));
		}
	};

	struct PulseSimplifier{
		I3MCPulse operator()(const AugmentedI3MCPulse& p) const{
			return(p);
		}
	};

	template<typename T>
	bool earlier(const T& h1, const T& h2){
		return(h1.time < h2.time);
	}
	
}

#include "dataclasses/physics/I3ParticleID.h"

/**
 * I3ParticleIDMap is used to describe the originating particles of I3MCPulses in
 * an I3MCPulseSeriesMap. For each OMKey a map of I3ParticleID to a list of
 * indices of MCPulses is stored, where the indices refer to the corresponding
 * I3MCPulseSeries within the associated I3MCPulseSeriesMap. By convention, each
 * list of indices is kept in sorted order.
 */
typedef std::map<I3ParticleID, std::vector<uint32_t> > ParticlePulseIndexMap;
typedef I3Map<OMKey,  ParticlePulseIndexMap> I3ParticleIDMap;
I3_POINTER_TYPEDEFS(I3ParticleIDMap);

//given N pulses derived from M particles, with A being the average number of parrent particles per pulse
//this function should run in (average) time proportional to N*M*log(A*N/M)
//and use additional space of approximately N*(sizeof(I3MCPulse)+4) + A*N*4 bytes
//  = N*(sizeof(I3MCPulse) + 4*(A+1)) bytes
//TODO: This code is horribly cumbersome. Can it be made less so?
template<typename ForwardIterator>
void sortMCPulses(ForwardIterator begin, ForwardIterator end, ParticlePulseIndexMap& aux){
	using namespace detail;
	//make copies of all of the pulses, augmented with their initial indices
	std::vector<AugmentedI3MCPulse> pulses;
	pulses.reserve(std::distance(begin,end));
	std::transform(begin,end,std::back_inserter(pulses),PulseAugmenter());
	
	//sort the augmented pulses
	std::sort(pulses.begin(),pulses.end(),earlier<AugmentedI3MCPulse>);
	
	//update the particle ID mappings
	uint32_t newIndex=0;
	ParticlePulseIndexMap newAux(aux);
	for(std::vector<AugmentedI3MCPulse>::const_iterator pulse=pulses.begin(), pulseEnd=pulses.end(); pulse!=pulseEnd; pulse++, newIndex++){
		uint32_t oldIndex=pulse->index;
		//find every instance of the old index, and replace it with the new index
		//(in the copy of aux, so as not to confuse later lookups of old indices)
		ParticlePulseIndexMap::iterator newParticle=newAux.begin();
		for(ParticlePulseIndexMap::const_iterator particle=aux.begin(), particleEnd=aux.end(); particle!=particleEnd; particle++, newParticle++){
			//if(oldIndex<particle->second.front() || oldIndex>particle->second.back())
			//	continue;
			std::vector<uint32_t>::const_iterator it=std::lower_bound(particle->second.begin(),particle->second.end(),oldIndex);
			if(it!=particle->second.end() && *it==oldIndex)
				newParticle->second[std::distance(particle->second.begin(),it)]=newIndex; //this should be efficient since particle->second.begin() and it are RandomAccess
		}
	}
	//replace old mappings with new
	aux.swap(newAux);
	//restore sorted order of mappings
	for(ParticlePulseIndexMap::iterator particle=aux.begin(), particleEnd=aux.end(); particle!=particleEnd; particle++)
		std::sort(particle->second.begin(),particle->second.end());
	
	//copy the pulses back to the original storage, leaving out the indices
	std::transform(pulses.begin(),pulses.end(),begin,PulseSimplifier());
}

std::vector<I3ParticleID> findParents(uint32_t pulseIndex, const ParticlePulseIndexMap& idMap);

#endif
