/**
 *  $Id$
 *  
 *  Copyright (C) 2008
 *  Troy D. Straszheim  <troy@icecube.umd.edu>
 *  and the IceCube Collaboration <http://www.icecube.wisc.edu>
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

#include <icetray/load_project.h>

void register_I3MCPMTResponse();
void register_I3MMCTrack();
void register_I3SumGenerator();
void register_CorsikaLongStep();
void register_I3CorsikaShowerInfo();
void register_MMCWeight();

BOOST_PYTHON_MODULE(simclasses)
{
  load_project("libsimclasses", false);

  register_I3MCPMTResponse();
  register_I3MMCTrack();
  register_I3SumGenerator();
  register_CorsikaLongStep();
  register_I3CorsikaShowerInfo();
  register_MMCWeight();
}

