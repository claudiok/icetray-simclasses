//
//   Copyright (c) 2013   Alex Olivas
//

#include <simclasses/I3MCPulse.h>
#include <icetray/python/dataclass_suite.hpp>

using namespace boost::python;

void register_I3MCPulse()
{
  {
    scope mcpulse_scope = 
      class_<I3MCPulse, boost::shared_ptr<I3MCPulse> >
      ("I3MCPulse")
      .def_readwrite("time",&I3MCPulse::time)
      .def_readwrite("source",&I3MCPulse::source)
      .def_readwrite("charge",&I3MCPulse::charge)
      .def(dataclass_suite<I3MCPulse>())
      ;

    enum_<I3MCPulse::PulseSource>("I3MCPulseSource")
      .value("UNKNOWN", I3MCPulse::UNKNOWN)
      .value("PE", I3MCPulse::PE)
      .value("RANDOM", I3MCPulse::RANDOM)
      .value("AFTER_PULSE", I3MCPulse::AFTER_PULSE)
      .value("PRE_PULSE", I3MCPulse::PRE_PULSE)
      .value("ELASTIC_LATE_PULSE", I3MCPulse::ELASTIC_LATE_PULSE)
      .value("INELASTIC_LATE_PULSE", I3MCPulse::INELASTIC_LATE_PULSE)
      .value("EARLY_AFTER_PULSE", I3MCPulse::EARLY_AFTER_PULSE)
      .export_values()
      ;
    def("identity", identity_<I3MCPulse::PulseSource>);  
  }

  class_<std::vector<I3MCPulse> >("vector_I3MCPulse")
    .def(dataclass_suite<std::vector<I3MCPulse> >())
    ;

  class_<I3MCPulseSeriesMap, bases<I3FrameObject>, I3MCPulseSeriesMapPtr>("I3MCPulseSeriesMap")
    .def(dataclass_suite<I3MCPulseSeriesMap>())
    ;
  
  register_pointer_conversions<I3MCPulseSeriesMap>();

}
