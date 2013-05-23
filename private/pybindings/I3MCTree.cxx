#include <dataclasses/physics/I3MCTree.h>
#include <simclasses/I3MCPE.h>
#include <sim-services/I3MCTreeUtils.h>

void register_I3MCTree()
{
  boost::python::def("get", &I3MCTreeUtils::Get);
}
