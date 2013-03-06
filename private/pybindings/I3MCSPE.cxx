//
//   Copyright (c) 2013   Alex Olivas
//

#include <simclasses/I3MCSPE.h>
#include <icetray/python/dataclass_suite.hpp>

using namespace boost::python;

void register_I3MCSPESeries()
{
  
  {
    scope mcspe_scope = 
      class_<I3MCSPE, boost::shared_ptr<I3MCSPE> >
      ("I3MCSPE")
      .add_property("time",&I3MCSPE::time)
      .add_property("npe",&I3MCSPE::npe)
      .def_readonly("major_ID",&I3MCSPE::major_ID)
      .def_readonly("minor_ID",&I3MCSPE::minor_ID)
      ;
  }

  class_<I3MCSPESeries, I3MCSPESeriesPtr>("I3MCSPESeries")
    .def(dataclass_suite<I3MCSPESeries>())
  ;    
  class_<I3MCSPESeriesMap, I3MCSPESeriesMapPtr>("I3MCSPESeriesMap")
    .def(dataclass_suite<I3MCSPESeriesMap>())
  ;    
  register_pointer_conversions<I3MCSPESeriesMap>();
}
