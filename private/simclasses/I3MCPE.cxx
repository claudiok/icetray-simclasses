#include <dataclasses/physics/I3Particle.h>
#include <simclasses/I3MCPE.h>

// constructor for hit makers
I3MCPE::I3MCPE(const I3Particle& p) :
  major_ID(p.GetMajorID()), 
  minor_ID(p.GetMinorID()){};

I3_SERIALIZABLE(I3MCPESeriesMap);

I3MCHit pe_to_hit(const I3MCPE&);
void pe_to_hit(const I3MCPE&, I3MCHit&);
I3MCPE hit_to_pe(const I3MCHit&);
void hit_to_pe(const I3MCHit&, I3MCPE&);
