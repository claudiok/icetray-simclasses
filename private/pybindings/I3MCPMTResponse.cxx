/**
 *  $Id$
 *  
 *  Copyright (C) 2007, 2008
 *  The IceCube Collaboration <http://www.icecube.wisc.edu>
 *  
 *  This file is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 3 of the License, or
 *  (at your option) any later version.
 *  
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *  
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>
 *  
 */

#include <simclasses/I3MCPMTResponse.h>
#include <icetray/python/std_map_indexing_suite.hpp>

using namespace boost::python;

void set_waveform_func(I3MCPMTResponsePtr p, std::vector<double> wf){
  p->GetWaveform() = wf;
}

void register_I3MCPMTResponse()
{
  //cstd::vector<double>& (I3MCPMTResponse::*get_waveform)() const = &I3MCPMTResponse::GetWaveform;
  //object get_waveform_func = make_function(get_waveform, return_internal_reference<1>());

  /* explicitly resolve overloads. */
  const double (I3MCPMTResponse::*get_start_time)() const = &I3MCPMTResponse::GetStartTime;
  const double (I3MCPMTResponse::*get_end_time)() const = &I3MCPMTResponse::GetEndTime;
  const double (I3MCPMTResponse::*get_bin_size)() const = &I3MCPMTResponse::GetBinSize;
  void (I3MCPMTResponse::*set_start_time)(double) = &I3MCPMTResponse::SetStartTime;
  void (I3MCPMTResponse::*set_end_time)(double) = &I3MCPMTResponse::SetEndTime;
  void (I3MCPMTResponse::*set_bin_size)(double) = &I3MCPMTResponse::SetBinSize;
  
 {
  class_<I3MCPMTResponse, I3MCPMTResponsePtr >
    ("I3MCPMTResponse")    
    .def("GetWaveform", &I3MCPMTResponse::GetWaveform)
    GETSET(I3MCPMTResponse, double, StartTime)
    GETSET(I3MCPMTResponse, double, EndTime)
    GETSET(I3MCPMTResponse, double, BinSize)
    .add_property("waveform",   &I3MCPMTResponse::GetWaveform, &I3MCPMTResponse::SetWaveform)
    .add_property("start_time", get_start_time, set_start_time)
    .add_property("end_time",   get_end_time,   set_end_time)
    .add_property("bin_size",   get_bin_size,   set_bin_size)
    ;

  class_<I3MCPMTResponseMap, bases<I3FrameObject>, I3MCPMTResponseMapPtr>("Map_OMKey_I3MCPMTResponse")
    .def(std_map_indexing_suite<I3MCPMTResponseMap>())
    ;

  register_pointer_conversions<I3MCPMTResponseMap>();
 }
}
