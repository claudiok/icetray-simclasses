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

using namespace boost::python;

void register_I3MCPMTResponse()
{
  const std::vector<double>& (I3MCPMTResponse::*get_waveform)() const = &I3MCPMTResponse::GetWaveform;
  object get_waveform_func = make_function(get_waveform, return_internal_reference<1>());
 {
  class_<I3MCPMTResponse, I3MCPMTResponsePtr >
    ("I3MCPMTResponse")    
    .def("GetWaveform", get_waveform_func)
    GETSET(I3MCPMTResponse, double, StartTime)
    GETSET(I3MCPMTResponse, double, EndTime)
    GETSET(I3MCPMTResponse, double, BinSize)
    .add_property("waveform", get_waveform_func)
    ;

  class_<I3MCPMTResponseMap, bases<I3FrameObject>, I3MCPMTResponseMapPtr>("Map_OMKey_I3MCPMTResponse")
    .def(map_indexing_suite<I3MCPMTResponseMap>())
    ;

  register_pointer_conversions<I3MCPMTResponseMap>();
 }
}
