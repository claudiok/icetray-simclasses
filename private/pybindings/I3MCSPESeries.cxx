//
//   Copyright (c) 2013   Alex Olivas
//

#include <simclasses/I3MCSPESeries.h>
#include <icetray/python/dataclass_suite.hpp>

using namespace boost::python;

void register_I3MCSPESeries()
{

  void (I3MCSPESeries::*fill_float)(float) = &I3MCSPESeries::fill;
  void (I3MCSPESeries::*fill_float_weight)(float, uint32_t) = &I3MCSPESeries::fill;
  void (I3MCSPESeries::*fill_float_vector)(const std::vector<float>& ) = 
    &I3MCSPESeries::fill;
  void (I3MCSPESeries::*fill_float_vector_vector)(const std::vector<float>&, 
						  const std::vector<uint32_t>&) = 
    &I3MCSPESeries::fill;
  
  {
    scope pehist_scope = 
      class_<I3MCSPESeries, boost::shared_ptr<I3MCSPESeries > >
      ("I3MCSPESeries")
      .def(init<float>())
      .def("fill", fill_float )
      .def("fill", fill_float_weight )
      .def("fill", fill_float_vector )
      .def("fill", fill_float_vector_vector )
      .add_property("bin_width",&I3MCSPESeries::get_bin_width)
      .add_property("npe_values",&I3MCSPESeries::npe_values)
      .add_property("arrival_times",&I3MCSPESeries::arrival_times)
      .add_property("is_binned",&I3MCSPESeries::is_binned)
      .def("sort", &I3MCSPESeries::sort)
      .def("__len__",&I3MCSPESeries::size)
      ;
  }

  class_<std::vector<float>, boost::shared_ptr<std::vector<float> > > 
      ("vector_float")
   .def(dataclass_suite<std::vector<float> >())
   ;

  class_<std::vector<int32_t>, boost::shared_ptr<std::vector<int32_t> > > 
      ("vector_int32_t")
   .def(dataclass_suite<std::vector<int32_t> >())
   ;

  class_<I3MCSPESeriesMap, I3MCSPESeriesMapPtr>("I3MCSPESeriesMap")
    .def(dataclass_suite<I3MCSPESeriesMap>())
  ;  

  class_<I3MCSPESeriesMapMap, bases<I3FrameObject>, I3MCSPESeriesMapMapPtr>
  ("I3MCSPESeriesMapMap")
    .def(dataclass_suite<I3MCSPESeriesMapMap>())
  ;
  
  register_pointer_conversions<I3MCSPESeriesMapMap>();
}
