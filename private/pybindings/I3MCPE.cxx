//
//   Copyright (c) 2013   Alex Olivas
//

#include <simclasses/I3MCPE.h>
#include <simclasses/I3MCPEUtils.h>
#include <icetray/python/dataclass_suite.hpp>

using namespace boost::python;

void register_I3MCPESeries()
{  
  {
    scope mcspe_scope = 
      class_<I3MCPE, boost::shared_ptr<I3MCPE> >("I3MCPE")
      .def(dataclass_suite<I3MCPE>())
      .def_readwrite("time",&I3MCPE::time)
      .def_readwrite("npe",&I3MCPE::npe)
      .def_readonly("ID",&I3MCPE::ID)
      ;
  }

  class_<I3MCPESeries, I3MCPESeriesPtr>("I3MCPESeries")
    .def(dataclass_suite<I3MCPESeries>())
  ;

  class_<I3MCPESeriesMap, I3MCPESeriesMapPtr, bases<I3FrameObject> >("I3MCPESeriesMap")
    .def(dataclass_suite<I3MCPESeriesMap>())
    .def("merge", &I3MCPEUtils::Merge, "Merge two MCPESeriesMaps")
    .def("sort", &I3MCPEUtils::SortMCPESeriesMap, "Sort MCPESeries of MCPESeriesMap")
    .def("merge_nosort", &I3MCPEUtils::MergeNoSort, "Merge two MCPESeries without sorting MCPESeries time")
  ;
  register_pointer_conversions<I3MCPESeriesMap>();
}
