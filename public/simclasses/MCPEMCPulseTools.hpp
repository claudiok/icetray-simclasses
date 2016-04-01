#ifndef MCPEMCPULSETOOLS_H_INCLUDED
#define MCPEMCPULSETOOLS_H_INCLUDED

#include <simclasses/I3MCPulse.h>
#include <simclasses/I3MCPE.h>
#include <simclasses/I3ParticleIDMap.hpp>

namespace {
  
  // class AugmentedI3MCPulse : public I3MCPulse{
  // public:
  //   uint32_t index;
  //   AugmentedI3MCPulse(const I3MCPulse& p, uint32_t i):I3MCPulse(p),index(i){}
  // };
  //
  // struct PulseAugmenter{
  // private:
  //   uint32_t index;
  // public:
  //   PulseAugmenter():index(0){}
  //
  //   AugmentedI3MCPulse operator()(const I3MCPulse& p){
  //     return(AugmentedI3MCPulse(p,index++));
  //   }
  // };
  //
  // struct PulseSimplifier{
  //   I3MCPulse operator()(const AugmentedI3MCPulse& p) const{
  //     return(p);
  //   }
  // };
    
    struct indexRange
    {
        std::vector<uint32_t>::iterator cur, end;
        indexRange(std::vector<uint32_t>& container):
        cur(container.begin()),end(container.end()){}
    };
    
    template<typename HitType>
    struct HitTraits{};
    
    template<>
    struct HitTraits<I3MCPE>
    {
        typedef uint32_t charge_type;
        typedef double time_type;
        static charge_type& getCharge(I3MCPE& h){ return(h.npe); }
        static time_type& getTime(I3MCPE& h){ return(h.time); }
        static const charge_type& getCharge(const I3MCPE& h){ return(h.npe); }
        static const time_type& getTime(const I3MCPE& h){ return(h.time); }
        /*static void printParent(const I3MCPE& h){
            std::cout << '(' << h.major_ID << ',' << h.minor_ID << ')';
        }
        static bool sameParent(const I3MCPE& h1, const I3MCPE& h2){
            return(h1.major_ID==h2.major_ID && h1.minor_ID==h2.minor_ID);
        }*/
    };
    
    template<>
    struct HitTraits<I3MCPulse>
    {
        typedef float charge_type;
        typedef double time_type;
        static charge_type& getCharge(I3MCPulse& h){ return(h.charge); }
        static time_type& getTime(I3MCPulse& h){ return(h.time); }
        static const charge_type& getCharge(const I3MCPulse& h){ return(h.charge); }
        static const time_type& getTime(const I3MCPulse& h){ return(h.time); }
        static void printParent(const I3MCPulse& h){}
        static bool sameParent(const I3MCPulse& h1, const I3MCPulse& h2){ return(true); }
    };
  
    template<typename T>
    bool earlier(const T& h1, const T& h2){
    return(h1.time < h2.time);
    }
  
    template<typename HitType>
    class AugmentedHit : public HitType
    {
        public:
            uint32_t index;
            AugmentedHit(const HitType& p, uint32_t i): HitType(p), index(i){}
    };
  
    template<typename HitType>
    struct HitAugmenter
    {
        private:
            uint32_t index;
        public:
            HitAugmenter():index(0){}
            
        AugmentedHit<HitType> operator()(const HitType& p)
        {
          return(AugmentedHit<HitType>(p,index++));
        }
    };
  
    template<typename HitType>
    struct HitSimplifier
    {
        HitType operator()(const AugmentedHit<HitType>& p) const
        {
          return(p);
        }
    };
  
}

namespace MCHitMerging
{

    // Merges together pulses which are within some small time window, compacting the hit series
    //  if it is densely filled with hits.
    // (Here, 'small' is intended to be relative to the resolution of the feature extractor.)
    // \param hits The hit series to be compacted
    // \pre The input hits must be time ordered
    template<typename HitType>
    void timeMergeHits(std::vector<HitType>& hits, ParticlePulseIndexMap& aux)
    {
        //This is the time span in which adjacent hits will be merged
        //The chosen value, 200 picoseconds, is based on the advice of
        //N. Whitehorn with regard to the highest resolution which wavedeform
        //achieves in unfolding pulses
        const double timeWindow = .2; //ns
    
        typedef typename HitTraits<HitType>::charge_type charge_type;
        typedef typename HitTraits<HitType>::time_type time_type;
    
        assert(!hits.empty());
    
        std::vector<indexRange> ranges;
        ParticlePulseIndexMap::iterator it, end;
        for(it = aux.begin(), end = aux.end(); it != end; it++)
            ranges.push_back(indexRange(it->second));
    
        uint32_t minOldIndex=0, maxOldIndex=1, newIndex=0, removed=0;
        typename std::vector<HitType>::iterator it1=hits.begin(), it2=hits.begin();
        typename std::vector<HitType> newHits;
        bool madeChanges=false;
        it2++;
        while(it1 != hits.end())
        {
            bool doMerge=false;
            charge_type charge1=HitTraits<HitType>::getCharge(*it1);
            time_type time1=HitTraits<HitType>::getTime(*it1);
            charge_type weightSum=charge1;
            time_type avgTime=time1;
            log_trace_stream("merge base time now " << avgTime);
            //std::cout << "   first index is " << minOldIndex << "; hit parent is ";
            //HitTraits<HitType>::printParent(*it1);
            //std::cout << '\n';
            //collect all hits which can be merged
            while(it2 != hits.end() && (it2->time-it1->time) <= timeWindow)
            {
                doMerge=true;
                charge_type charge2=HitTraits<HitType>::getCharge(*it2);
                time_type time2=HitTraits<HitType>::getTime(*it2);
                log_trace_stream("will merge hit " << time2-time1 << " ns later");
                //std::cout << "    merging with hit at index " << maxOldIndex << " whose parent is ";
                //HitTraits<HitType>::printParent(*it2);
                //std::cout << '\n';
                //if(HitTraits<HitType>::sameParent(*it1,*it2))
                //  std::cout << "     DING!\n";
                avgTime=(weightSum*avgTime + charge2*time2)/(weightSum + charge2);
                weightSum+=charge2;
                it2++;
                maxOldIndex++;
            }
            //Merge the set of hits if necessary
            if(doMerge)
            {
                removed+=maxOldIndex-minOldIndex-1;
                if(!madeChanges)
                { //copy all of the unmerged hits that were skipped previously
                    std::copy(hits.begin(),it1,std::back_inserter(newHits));
                    newIndex+=std::distance(hits.begin(),it1);
                    //advance the aux iterators
                    for(std::vector<indexRange>::iterator ait=ranges.begin(), aitEnd=ranges.end(); ait!=aitEnd; ait++)
                        ait->cur=std::lower_bound(ait->cur,ait->end,newIndex);
                    madeChanges=true;
                }
                log_trace_stream("merged hit time is " << avgTime << ", and weight is " << weightSum);
                newHits.push_back(*it1);
                HitTraits<HitType>::getTime(newHits.back())=avgTime;
                HitTraits<HitType>::getCharge(newHits.back())=weightSum;
                //update aux, replacing all indices in [minOldIndex,maxOldIndex) with newIndex
                std::vector<indexRange>::iterator ait, aitEnd;
                for(ait=ranges.begin(), aitEnd=ranges.end(); ait!=aitEnd; ait++)
                {
                    while(ait->cur!=ait->end && *ait->cur<maxOldIndex)
                    {
                        *(ait->cur)=newIndex; //overwrite the index with the index of the merged pulse
                        ait->cur++; //advance to the next index
                    }
                }
                newIndex++;
            }
            else if(madeChanges)
            {
                std::copy(it1,it2,std::back_inserter(newHits));
                std::vector<indexRange>::iterator ait, aitEnd;
                for(ait = ranges.begin(), aitEnd = ranges.end(); ait != aitEnd; ait++){
                    while(ait->cur!=ait->end && *ait->cur<maxOldIndex){
                        *(ait->cur)-=removed; //decrease the index by the number of pulses removed
                        ait->cur++; //advance to the next index
                    }
                }
                newIndex+=std::distance(it1,it2);
            }
            it1=it2++; //skip to the next possible group
            minOldIndex=maxOldIndex++;
        }
        if(madeChanges){
            hits.swap(newHits);
            //deduplicate the contents of aux
            ParticlePulseIndexMap::iterator it, end;
            for(it = aux.begin(), end = aux.end(); it != end; it++){
                //std::cout << " Particle (" << it->first.majorID << ',' << it->first.minorID << ")\n";
                std::vector<uint32_t> temp;
                std::unique_copy(it->second.begin(),it->second.end(),std::back_inserter(temp));
                //std::cout << "  old size: " << it->second.size() << ", new size: " << temp.size() << std::endl;
                //std::cout << "  old capacity: " << it->second.capacity() << ", new capacity: " << temp.capacity() << std::endl;
                it->second.swap(temp);
            }
        }
    }

    //move particle parentage information out into a separate data structure
    //erase particle information from the hits to prevent later confusion
    //run the time merging
    // boost::shared_ptr<I3ParticleIDMap> extractPIDandMerge(boost::shared_ptr<I3Map<OMKey,std::vector<I3MCPE> > > hits)
    I3ParticleIDMapPtr extractPIDandMerge(boost::shared_ptr<I3Map<OMKey,std::vector<I3MCPE> > > hits);
    
    ParticlePulseIndexMap extractPIDandMerge(std::vector<I3MCPE>& hits, OMKey key);
    
    std::vector<I3ParticleID> findParents(uint32_t pulseIndex, const ParticlePulseIndexMap& idMap);
    
    
    // Given N pulses derived from M particles, with A being the average number
    // of parrent particles per pulse this function should run in (average) time 
    // proportional to N*M*log(A*N/M) and use additional space of approximately 
    // N*(sizeof(I3MCPulse)+4) + A*N*4 bytes = N*(sizeof(I3MCPulse) + 4*(A+1)) bytes
    //TODO: This code is horribly cumbersome. Can it be made less so?
    template<typename ForwardIterator>
    void sortMCHits(ForwardIterator begin, ForwardIterator end, ParticlePulseIndexMap& aux)
    {
        //make copies of all of the pulses, augmented with their initial indices
        // std::vector<AugmentedI3MCPulse> pulses;
        typedef typename std::iterator_traits<ForwardIterator>::value_type HitType;
        std::vector<AugmentedHit<HitType> > hits;
        hits.reserve(std::distance(begin,end));
        std::transform(begin, end, std::back_inserter(hits), HitAugmenter<HitType>());

        //sort the augmented hits
        std::sort(hits.begin(),hits.end(), earlier<AugmentedHit<HitType> >);

        //update the particle ID mappings
        uint32_t newIndex=0;
        ParticlePulseIndexMap newAux(aux);
        typename std::vector<AugmentedHit<HitType> >::const_iterator pulse, pulseEnd;
        for(pulse=hits.begin(), pulseEnd=hits.end(); 
            pulse!=pulseEnd; 
            pulse++, newIndex++)
            {
                uint32_t oldIndex=pulse->index;
                //find every instance of the old index, and replace it with the new index
                //(in the copy of aux, so as not to confuse later lookups of old indices)
                ParticlePulseIndexMap::iterator newParticle=newAux.begin();
                for(ParticlePulseIndexMap::const_iterator particle=aux.begin(), particleEnd=aux.end(); 
                    particle!=particleEnd; 
                    particle++, newParticle++)
                    {
                        std::vector<uint32_t>::const_iterator it = std::lower_bound(particle->second.begin(),
                                                                      particle->second.end(),
                                                                      oldIndex);
                        if(it!=particle->second.end() && *it==oldIndex)
                        //this should be efficient since particle->second.begin() and it are RandomAccess
                            newParticle->second[std::distance(particle->second.begin(),it)] = newIndex; 
                    }
            }
        //replace old mappings with new
        aux.swap(newAux);
        //restore sorted order of mappings
        for(ParticlePulseIndexMap::iterator particle=aux.begin(), particleEnd=aux.end(); 
            particle!=particleEnd; 
            particle++)
                std::sort(particle->second.begin(), particle->second.end());

        //copy the hits back to the original storage, leaving out the indices
        std::transform(hits.begin(), hits.end(), begin, HitSimplifier<HitType>());
    }

}

#endif
