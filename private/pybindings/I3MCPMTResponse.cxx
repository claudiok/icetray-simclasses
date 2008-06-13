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

double get_start_time(I3MCPMTResponsePtr p){
  return p->GetStartTime();
}

double get_end_time(I3MCPMTResponsePtr p){
  return p->GetEndTime();
}

double get_bin_size(I3MCPMTResponsePtr p){
  return p->GetBinSize();
}

vector<double> get_waveform(I3MCPMTResponsePtr p){
  return p->GetWaveform();
}

void register_I3MCPMTResponse()
{
  class_<I3MCPMTResponse, boost::shared_ptr<I3MCPMTResponse> >
    ("I3MCPMTResponse")    
    .def("GetStartTime", get_start_time)
    .def("GetEndTime", get_end_time)
    .def("GetBinWidth", get_bin_size)
    .def("GetWaveform", get_waveform)
    ;

  class_<I3MCPMTResponseMap, bases<I3FrameObject>, I3MCPMTResponseMapPtr>("Map_OMKey_I3MCPMTResponse")
    .def(map_indexing_suite<I3MCPMTResponseMap>())
    ;

  register_pointer_conversions<I3MCPMTResponseMap>();
}
