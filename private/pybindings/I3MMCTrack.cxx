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

#include <simclasses/I3MMCTrack.h>

using namespace boost::python;

bool operator==(const I3MMCTrack& lhs, const I3MMCTrack& rhs){
  return 
    lhs.GetXi() == rhs.GetXi() ||
    lhs.GetYi() == rhs.GetYi() ||
    lhs.GetZi() == rhs.GetZi() ||
    lhs.GetTi() == rhs.GetTi() ||
    lhs.GetEi() == rhs.GetEi() ||
    lhs.GetXc() == rhs.GetXc() ||
    lhs.GetYc() == rhs.GetYc() ||
    lhs.GetZc() == rhs.GetZc() ||
    lhs.GetTc() == rhs.GetTc() ||
    lhs.GetEc() == rhs.GetEc() ||
    lhs.GetXf() == rhs.GetXf() ||
    lhs.GetYf() == rhs.GetYf() ||
    lhs.GetZf() == rhs.GetZf() ||
    lhs.GetTf() == rhs.GetTf() ||
    lhs.GetEf() == rhs.GetEf() ;
}

void register_I3MMCTrack()
{
  class_<I3MMCTrack, boost::shared_ptr<I3MMCTrack> >
    ("I3MMCTrack")    
    .def("GetXi", &I3MMCTrack::GetXi )
    .def("GetYi", &I3MMCTrack::GetYi )
    .def("GetZi", &I3MMCTrack::GetZi )
    .def("GetEi", &I3MMCTrack::GetEi )
    .def("GetTi", &I3MMCTrack::GetTi )
    .def("GetXc", &I3MMCTrack::GetXc )
    .def("GetYc", &I3MMCTrack::GetYc )
    .def("GetZc", &I3MMCTrack::GetZc )
    .def("GetEc", &I3MMCTrack::GetEc )
    .def("GetTc", &I3MMCTrack::GetTc )
    .def("GetXf", &I3MMCTrack::GetXf )
    .def("GetYf", &I3MMCTrack::GetYf )
    .def("GetZf", &I3MMCTrack::GetZf )
    .def("GetEf", &I3MMCTrack::GetEf )
    .def("GetTf", &I3MMCTrack::GetTf )
    .def("GetElost", &I3MMCTrack::GetElost )
    .def("GetI3Particle", &I3MMCTrack::GetI3Particle, return_internal_reference<1>() )
    ;

  class_<I3MMCTrackList, bases<I3FrameObject> >("I3MMCTrackList")
    .def(vector_indexing_suite<I3MMCTrackList >())
    ;

  register_pointer_conversions<I3MMCTrackList>();
}

