#ifndef I3MCPEUTILS_H_INCLUDED
#define I3MCPEUTILS_H_INCLUDED

#include "simclasses/I3MCPE.h"

namespace I3MCPESeriesMapUtils
{
   /* 
    * MergeNoSort
    * Merges two I3MCPEMaps without sorting MCPESeries in time
    * Shamelessly adapted from vuvuzela.
    *
    *  \param originalMap The MCPE series map to merge into
    *  \param mergeMap The MCPE series map from the frame
    */ 
    void MergeNoSort(I3MCPESeriesMap&, const I3MCPESeriesMap&);
    
   /* 
    * Merge
    * Merges two I3MCPEMaps. 
    * Shamelessly adapted from vuvuzela.
    *
    *  \param originalMap The MCPE series map to merge into
    *  \param mergeMap The MCPE series map from the frame
    */ 
    void Merge(I3MCPESeriesMap&, const I3MCPESeriesMap&);

   /* 
    * MergeReturn
    * Merges two I3MCPEMaps . 
    * Shamelessly adapted from vuvuzela.
    *
    *  \param originalMap The MCPE series map to merge into
    *  \param mergeMap The MCPE series map from the frame
    *  \return New merged MCPE series map
    */ 
    I3MCPESeriesMapConstPtr MergeReturn(const I3MCPESeriesMap& originalMap, const I3MCPESeriesMap& mergeMap);
   
   /* 
    * SortMCPESeriesMap
    * Sort MCPESeries of a MCPESeriesMap in time. 
    * Shamelessly adapted from vuvuzela.
    *
    *  \param unsortedMap MCPE series map not sorted in time
    */ 
    void SortMCPESeriesMap(I3MCPESeriesMap&);

}

#endif 
