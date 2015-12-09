#ifndef I3MCPEUTILS_H_INCLUDED
#define I3MCPEUTILS_H_INCLUDED

#include "simclasses/I3MCPE.h"

namespace I3MCPESeriesMapUtils
{

    void MergeNoSort(I3MCPESeriesMap&, const I3MCPESeriesMap&);

    void Merge(I3MCPESeriesMap&, const I3MCPESeriesMap&);

    I3MCPESeriesMapConstPtr MergeReturn(const I3MCPESeriesMap& originalMap, const I3MCPESeriesMap& mergeMap);

    void SortMCPESeriesMap(I3MCPESeriesMap&);

}

#endif 
