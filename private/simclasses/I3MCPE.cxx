#include <simclasses/I3MCPE.h>
#include <ostream>
#include "I3SequenceOpOStream.h"

I3_SERIALIZABLE(I3MCPESeriesMap);

std::ostream& operator<<(std::ostream& os, const I3MCPE& pe) {
  os << "[ I3MCPE::" << std::endl
     << "  Time :" << pe.time << std::endl
     << "  NPE  :" << pe.npe << std::endl
     << "  " << pe.ID << std::endl
     << "]";
  return os;
}

I3_SEQUENCE_OP_OSTREAM(I3MCPESeries, "\n ");
