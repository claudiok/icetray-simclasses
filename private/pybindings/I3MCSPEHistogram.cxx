//
//   Copyright (c) 2013   Alex Olivas
//

#include <dataclasses/physics/I3MCSPEHistogram.h>
#include <icetray/python/dataclass_suite.hpp>

using namespace boost::python;

void register_I3MCSPEHistogram()
{

  void (I3MCSPEHistogram<float>::*fill_float)(float) = &I3MCSPEHistogram<float>::fill;
  void (I3MCSPEHistogram<float>::*fill_float_weight)(float, uint32_t) = &I3MCSPEHistogram<float>::fill;
  void (I3MCSPEHistogram<float>::*fill_float_vector)(const std::vector<float>& ) = 
    &I3MCSPEHistogram<float>::fill;
  
  {
    scope pehist_scope = 
      class_<I3MCSPEHistogram<float>, boost::shared_ptr<I3MCSPEHistogram<float> > >
      ("I3MCSPEHistogramFloat")
      .def(init<float>())
      .def(init<float,const std::vector<float>& >())
      .def("fill", fill_float )
      .def("fill", fill_float_weight )
      .def("fill", fill_float_vector )
      .add_property("bin_values",&I3MCSPEHistogram<float>::bin_values)
      .add_property("leading_edges",&I3MCSPEHistogram<float>::leading_edges)
      .add_property("is_binned",&I3MCSPEHistogram<float>::is_binned)
      //.def(dataclass_suite<I3MCSPEHistogram<> >())
      ;
  }

  class_<std::vector<float>, boost::shared_ptr<std::vector<float> > > 
      ("vector_float")
   .def(dataclass_suite<std::vector<float> >())
   ;

}
