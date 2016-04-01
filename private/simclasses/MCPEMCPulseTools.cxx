#include <simclasses/MCPEMCPulseTools.hpp>

namespace MCHitMerging
{

    std::vector<I3ParticleID> findParents(uint32_t pulseIndex, const ParticlePulseIndexMap& idMap){
      std::vector<I3ParticleID> results;
      for(ParticlePulseIndexMap::const_iterator particle=idMap.begin(), particleEnd=idMap.end(); particle!=particleEnd; particle++){
        std::vector<uint32_t>::const_iterator it=std::lower_bound(particle->second.begin(),particle->second.end(),pulseIndex);
        if(it!=particle->second.end() && *it==pulseIndex)
          results.push_back(particle->first);
      }
      return(results);
    }
    
    ParticlePulseIndexMap extractPIDandMerge(std::vector<I3MCPE>& hits, OMKey key)
    {
        ParticlePulseIndexMap particleMap;
        uint32_t counter = 0;
        double lastTime =- std::numeric_limits<double>::max();
        I3MCPESeries::iterator hitIt, hitEnd;
        for(hitIt = hits.begin(), hitEnd = hits.end(); hitIt != hitEnd; hitIt++, counter++)
        {
            I3ParticleID pid = {hitIt->ID.majorID, hitIt->ID.minorID};
            particleMap[pid].push_back(counter);
            hitIt->ID.majorID = 0;
            hitIt->ID.minorID = -1;
            //while we're here, check time ordering
            if(hitIt->time < lastTime)
                log_fatal_stream("MCPEs on " << key << " are not time ordered: " << lastTime << ", " << hitIt->time);
            lastTime = hitIt->time;
        }
        // now we know that the hits are time ordered, and we have the aux structure ready
        timeMergeHits(hits, particleMap);
        log_trace_stream("compressed " << counter << " p.e. on " << key << " to " << hits.size());
        //std::cout << "compressed " << counter << " p.e. on " << domIt->first << " to " << domIt->second.size() << std::endl;
        // unsigned int entries=0;
        // I3ParticleIDMap::iterator it, endIt;
        // for (it = resultPIDMap->begin(), endIt = resultPIDMap->end(); it != endIt; it++)
        //     entries += it->second.size();

        // for(const auto& particleRecord : particleMap)
        //         entries += particleRecord.second.size();
        //std::cout << " stored " << entries << " pulse entries from " << particleMap.size() << " particles" << std::endl;

        return(particleMap);
    }
    
    I3ParticleIDMapPtr extractPIDandMerge(boost::shared_ptr<I3Map<OMKey,std::vector<I3MCPE> > > hits)
    {
        I3ParticleIDMapPtr resultPIDMap(new I3ParticleIDMap());
        I3MCPESeriesMap::iterator domIt, domEnd;
        for(domIt = hits->begin(), domEnd = hits->end(); domIt != domEnd; domIt++)
        {
            ParticlePulseIndexMap& particleMap = (*resultPIDMap)[domIt->first];
            uint32_t counter = 0;
            double lastTime =- std::numeric_limits<double>::max();
            I3MCPESeries::iterator hitIt, hitEnd;
            for(domIt->second.begin(), hitEnd=domIt->second.end(); hitIt != hitEnd; hitIt++, counter++)
            {
                I3ParticleID pid = {hitIt->ID.majorID, hitIt->ID.minorID};
                particleMap[pid].push_back(counter);
                hitIt->ID.majorID = 0;
                hitIt->ID.minorID = -1;
                //while we're here, check time ordering
                if(hitIt->time<lastTime)
                    log_fatal_stream("MCPEs on " << domIt->first << " are not time ordered: " << lastTime << ", " << hitIt->time);
                lastTime = hitIt->time;
            }
            // now we know that the hits are time ordered, and we have the aux structure ready
            timeMergeHits(domIt->second, particleMap);
            log_trace_stream("compressed " << counter << " p.e. on " << domIt->first << " to " << domIt->second.size());
            //std::cout << "compressed " << counter << " p.e. on " << domIt->first << " to " << domIt->second.size() << std::endl;
            unsigned int entries=0;
            I3ParticleIDMap::iterator it, endIt;
            for (it = resultPIDMap->begin(), endIt = resultPIDMap->end(); it != endIt; it++)
                entries += it->second.size();

            // for(const auto& particleRecord : particleMap)
            //         entries += particleRecord.second.size();
            //std::cout << " stored " << entries << " pulse entries from " << particleMap.size() << " particles" << std::endl;
        }

        return(resultPIDMap);
    }
}