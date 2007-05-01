#include <icetray/serialization.h>
#include <simclasses/I3MCPMTResponse.h>

template <class Archive>
    void I3MCPMTResponse::serialize(Archive& ar, unsigned version){
    ar & make_nvp("Waveform",waveform_);
    ar & make_nvp("BinSize",binSize_);
    ar & make_nvp("startTime",startTime_);
    ar & make_nvp("endTime",endTime_);
  }

I3_SERIALIZABLE(I3MCPMTResponseMap);
