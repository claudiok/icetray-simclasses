/**
 *  $Id$
 *  
 *  Copyright (C) 2007
 *  the IceCube Collaboration <http://www.icecube.wisc.edu>
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
#include <dataclasses/ParticleIDKey.h>
#include <dataclasses/physics/I3Particle.h>
#include <icetray/python/dataclass_suite.hpp>

using namespace boost::python;

void register_ParticleIDKey()
{
  scope pidkey_scope = 
    class_<ParticleIDKey, boost::shared_ptr<ParticleIDKey> >("ParticleIDKey")
    .def(init<const I3Particle&>())
    .def(dataclass_suite<ParticleIDKey>())
    .def(self == self)
    .def(self < self)
    .add_property("major_id", &ParticleIDKey::GetMajorID, &ParticleIDKey::SetMajorID)
    .add_property("minor_id", &ParticleIDKey::GetMinorID, &ParticleIDKey::SetMinorID)
    ;
}
    
