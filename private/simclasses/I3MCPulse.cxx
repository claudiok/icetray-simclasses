#include <simclasses/I3MCPulse.h>

I3_SERIALIZABLE(I3MCPulseSeriesMap);

std::vector<I3ParticleID> findParents(uint32_t pulseIndex, const std::map<I3ParticleID,std::vector<uint32_t> >& idMap){
	std::vector<I3ParticleID> results;
	for(std::map<I3ParticleID,std::vector<uint32_t> >::const_iterator particle=idMap.begin(), particleEnd=idMap.end(); particle!=particleEnd; particle++){
		std::vector<uint32_t>::const_iterator it=std::lower_bound(particle->second.begin(),particle->second.end(),pulseIndex);
		if(it!=particle->second.end() && *it==pulseIndex)
			results.push_back(particle->first);
	}
	return(results);
}
