#include <simclasses/I3ParticleID.h>
#include <icetray/python/dataclass_suite.hpp>

using namespace boost::python;

void register_I3ParticleID()
{
	class_<I3ParticleID, boost::shared_ptr<I3ParticleID> >("I3ParticleID")
	.def_readwrite("majorID",&I3ParticleID::majorID)
	.def_readwrite("minorID",&I3ParticleID::minorID)
	;
	
	class_<I3Map<OMKey, std::map<I3ParticleID, std::vector<uint32_t> > >,
	       bases<I3FrameObject>,
	       I3ParticleIDMapPtr>("I3ParticleIDMap")
    .def(dataclass_suite<I3Map<OMKey, std::map<I3ParticleID, std::vector<uint32_t> > > >())
    ;
	
	register_pointer_conversions<I3ParticleIDMap>();
}