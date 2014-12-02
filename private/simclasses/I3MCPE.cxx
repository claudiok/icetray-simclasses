#include <simclasses/I3MCPE.h>
#include <ostream>

I3_SERIALIZABLE(I3MCPESeriesMap);

std::ostream& operator<<(std::ostream& os, const I3MCPE& pe) {
  os << "[ I3MCPE::"
     << "\n  Time       :" << pe.time
     << "\n  NPE        :" << pe.npe
     << "\n  ParticleID :" << pe.major_ID << "/" << pe.minor_ID << " ]\n";
  return os;
}
