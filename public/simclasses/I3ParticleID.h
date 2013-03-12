#ifndef I3ParticleID_H_INCLUDED
#define I3ParticleID_H_INCLUDED

#include <icetray/serialization.h>
#include <dataclasses/I3Map.h>

/**
 * @brief A single object for holding the ID numbers of an I3Particle
 */

struct I3ParticleID{
	uint64_t majorID;
	int32_t minorID;
	
	bool operator<(const I3ParticleID& other) const {
		return(majorID<other.majorID);
	}
	
private:
	friend class boost::serialization::access;
		
	template <class Archive> void serialize(Archive & ar, const unsigned version)
	{
		ar & make_nvp("majorID",majorID);
		ar & make_nvp("minorID",minorID);
	}
};

/**
 * I3ParticleIDMap is used to describe the originating particles of I3MCPulses in 
 * an I3MCPulseSeriesMap. For each OMKey a map of I3ParticleID to a list of
 * indices of MCPulses is stored, where the indices refer to the corresponding 
 * I3MCPulseSeries within the associated I3MCPulseSeriesMap. By convention, each
 * list of indices is kept in sorted order. 
 */
typedef I3Map<OMKey, std::map<I3ParticleID, std::vector<uint32_t> > > I3ParticleIDMap;
I3_POINTER_TYPEDEFS(I3ParticleIDMap);

#endif