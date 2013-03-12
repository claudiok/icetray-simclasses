//
//   Copyright (c) 2013   Alex Olivas
//

#include <simclasses/I3MCPE.h>
#include <icetray/python/dataclass_suite.hpp>

using namespace boost::python;

void register_I3MCPESeries()
{
  
  {
    scope mcspe_scope = 
      class_<I3MCPE, boost::shared_ptr<I3MCPE> >
      ("I3MCPE")
      .add_property("time",&I3MCPE::time)
      .add_property("npe",&I3MCPE::npe)
      .def_readonly("major_ID",&I3MCPE::major_ID)
      .def_readonly("minor_ID",&I3MCPE::minor_ID)
      ;
  }

  class_<I3MCPESeries, I3MCPESeriesPtr>("I3MCPESeries")
    .def(dataclass_suite<I3MCPESeries>())
  ;    
  class_<I3MCPESeriesMap, I3MCPESeriesMapPtr, bases<I3FrameObject> >("I3MCPESeriesMap")
    .def(dataclass_suite<I3MCPESeriesMap>())
  ;
  register_pointer_conversions<I3MCPESeriesMap>();
}
