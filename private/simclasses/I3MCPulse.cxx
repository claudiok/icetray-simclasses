#include <simclasses/I3MCPulse.h>
#include <ostream>
#include "I3SequenceOpOStream.h"

I3_SERIALIZABLE(I3MCPulseSeriesMap);

std::vector<I3ParticleID> findParents(uint32_t pulseIndex, const ParticlePulseIndexMap& idMap){
	std::vector<I3ParticleID> results;
	for(ParticlePulseIndexMap::const_iterator particle=idMap.begin(), particleEnd=idMap.end(); particle!=particleEnd; particle++){
		std::vector<uint32_t>::const_iterator it=std::lower_bound(particle->second.begin(),particle->second.end(),pulseIndex);
		if(it!=particle->second.end() && *it==pulseIndex)
			results.push_back(particle->first);
	}
	return(results);
}

//#warning I3ParticleIDMap serialization is currently broken and disabled!
I3_SERIALIZABLE(I3ParticleIDMap);

std::ostream& operator<<(std::ostream& os, const I3MCPulse& pulse) {
    os << "[ I3MCPulse::"
       << "\n  Time   :" << pulse.time 
       << "\n  Charge :" << pulse.charge
       << "\n  Source :" << pulse.source << " ]";
    return os;
}

I3_SEQUENCE_OP_OSTREAM(I3MCPulseSeries, "\n ");
