#include "simclasses/I3MCPEUtils.h"
/* 
 ********************************************************************
 * MergeNoSort                                                
 * Merges two I3MCPEMaps without sorting MCPESeries in time
 * Shamelessly adapted from vuvuzela.
 *
 *  \param originalMap The MCPE series map to merge into
 *  \param mergeMap The MCPE series map from the frame
 ******************************************************************** 
 */ 
void I3MCPESeriesMapUtils::MergeNoSort(I3MCPESeriesMap& originalMap, const I3MCPESeriesMap& mergeMap)
{
    I3MCPESeriesMap::const_iterator itMerge;
    // Loop over the noise hits
    for(itMerge = mergeMap.begin(); itMerge != mergeMap.end(); ++itMerge)
    {
        I3MCPESeriesMap::const_iterator itOriginal = originalMap.find(itMerge->first);
        if (itOriginal != originalMap.end())
        {
            assert(!(itMerge->second.empty()));
            const I3MCPESeries& mergeMCPESeries = itMerge->second;
            I3MCPESeries::const_iterator mergeMCPEIter;
            for(mergeMCPEIter = mergeMCPESeries.begin(); 
                mergeMCPEIter != mergeMCPESeries.end(); 
                ++mergeMCPEIter) 
              originalMap[itMerge->first].push_back(*mergeMCPEIter);
            
        }
        else if (!(itMerge->second.empty()))
        {
          originalMap.insert(std::make_pair(itMerge->first, itMerge->second));
        }
    }
}

bool CompareMCPEs(const I3MCPE& lhs, const I3MCPE& rhs){
  return lhs.time < rhs.time;
}

/* 
 ********************************************************************
 * SortMCPESeriesMap                                                
 * Sort MCPESeries of a MCPESeriesMap in time. 
 * Shamelessly adapted from vuvuzela.
 *
 *  \param unsortedMap MCPE series map not sorted in time
 ******************************************************************** 
 */ 
void I3MCPESeriesMapUtils::SortMCPESeriesMap(I3MCPESeriesMap& unsortedMap)
{
    I3MCPESeriesMap::iterator it;
    for (it = unsortedMap.begin(); it != unsortedMap.end(); ++it)
        sort((it->second).begin(), (it->second).end(), CompareMCPEs);
}

/* 
 ********************************************************************
 * Merge                                                
 * Merges two I3MCPEMaps . 
 * Shamelessly adapted from vuvuzela.
 *
 *  \param originalMap The MCPE series map to merge into
 *  \param mergeMap The MCPE series map from the frame
 ******************************************************************** 
 */ 
void I3MCPESeriesMapUtils::Merge(I3MCPESeriesMap& originalMap, const I3MCPESeriesMap& mergeMap)
{
    I3MCPESeriesMapUtils::MergeNoSort(originalMap, mergeMap);
    I3MCPESeriesMapUtils::SortMCPESeriesMap(originalMap);
    
}

/* 
 ********************************************************************
 * MergeReturn                                                
 * Merges two I3MCPEMaps . 
 * Shamelessly adapted from vuvuzela.
 *
 *  \param originalMap The MCPE series map to merge into
 *  \param mergeMap The MCPE series map from the frame
 *  \return New merged MCPE series map
 ******************************************************************** 
 */ 
I3MCPESeriesMapConstPtr I3MCPESeriesMapUtils::MergeReturn(const I3MCPESeriesMap& originalMap, 
                                                 const I3MCPESeriesMap& mergeMap)
{
    I3MCPESeriesMapPtr firstMap(new I3MCPESeriesMap(originalMap));
    I3MCPESeriesMapUtils::Merge(*firstMap, mergeMap);
    return I3MCPESeriesMapConstPtr(firstMap);
}
