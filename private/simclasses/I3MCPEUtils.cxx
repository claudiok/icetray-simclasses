#include "simclasses/I3MCPEUtils.h"

void I3MCPESeriesMapUtils::MergeNoSort(I3MCPESeriesMap& originalMap, const I3MCPESeriesMap& mergeMap)
{
    I3MCPESeriesMap::const_iterator itMerge;
    for(itMerge = mergeMap.begin(); itMerge != mergeMap.end(); ++itMerge)
    {
        I3MCPESeriesMap::const_iterator itOriginal = originalMap.find(itMerge->first);
        if (itOriginal != originalMap.end())
        {
            assert(!(itMerge->second.empty()));
            itOriginal->second.insert(itOriginal->second.end(),
                                      itMerge->second.begin(),
                                      itMerge->second.end());
            // const I3MCPESeries& mergeMCPESeries = itMerge->second;
            // I3MCPESeries::const_iterator mergeMCPEIter;
            // for(mergeMCPEIter = mergeMCPESeries.begin();
            //     mergeMCPEIter != mergeMCPESeries.end();
            //     ++mergeMCPEIter)
            //   originalMap[itMerge->first].push_back(*mergeMCPEIter);
            
        }
        else if (!(itMerge->second.empty()))
        {
          originalMap.insert(std::make_pair(itMerge->first, itMerge->second));
        }
    }
}

namespace
{
    bool CompareMCPEs(const I3MCPE& lhs, const I3MCPE& rhs)
    {
        return lhs.time < rhs.time;
    }
}



void I3MCPESeriesMapUtils::SortMCPESeriesMap(I3MCPESeriesMap& unsortedMap)
{
    I3MCPESeriesMap::iterator it;
    for (it = unsortedMap.begin(); it != unsortedMap.end(); ++it)
        sort((it->second).begin(), (it->second).end(), CompareMCPEs);
}

void I3MCPESeriesMapUtils::Merge(I3MCPESeriesMap& originalMap, const I3MCPESeriesMap& mergeMap)
{
    I3MCPESeriesMapUtils::MergeNoSort(originalMap, mergeMap);
    I3MCPESeriesMapUtils::SortMCPESeriesMap(originalMap);
    
}

I3MCPESeriesMapConstPtr I3MCPESeriesMapUtils::MergeReturn(const I3MCPESeriesMap& originalMap, 
                                                 const I3MCPESeriesMap& mergeMap)
{
    I3MCPESeriesMapPtr firstMap(new I3MCPESeriesMap(originalMap));
    I3MCPESeriesMapUtils::Merge(*firstMap, mergeMap);
    return I3MCPESeriesMapConstPtr(firstMap);
}
