#include <icetray/serialization.h>
#include <simclasses/I3MCTWRParams.h>

I3MCTWRParams::~I3MCTWRParams() {}

template <class Archive>
  void I3MCTWRParams::serialize(Archive& ar, unsigned version)
  {
    ar & make_nvp("stop_delay",stop_delay);
    ar & make_nvp("DMADD_thresh",DMADD_thresh);
    ar & make_nvp("TWR_thresh",TWR_thresh);
    ar & make_nvp("rel_sens",rel_sens);
    ar & make_nvp("wf_type",wf_type);
    ar & make_nvp("afterpulse_prob",afterpulse_prob);
    ar & make_nvp("afterpulse_t",afterpulse_time);
    ar & make_nvp("noise_rate",noise_rate);
    ar & make_nvp("amplitude",amplitude);
    ar & make_nvp("cable_delay",cable_delay);
    ar & make_nvp("optical",optical);
  }



I3_SERIALIZABLE(I3MCTWRParams);
