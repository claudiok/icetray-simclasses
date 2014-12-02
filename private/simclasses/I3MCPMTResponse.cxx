#include <icetray/serialization.h>
#include <simclasses/I3MCPMTResponse.h>

template <class Archive>
    void I3MCPMTResponse::serialize(Archive& ar, unsigned version){
    ar & make_nvp("Waveform",waveform_);
    ar & make_nvp("BinSize",binSize_);
    ar & make_nvp("startTime",startTime_);
    ar & make_nvp("endTime",endTime_);
  }

std::ostream& operator<<(std::ostream& os, const I3MCPMTResponse& r) {
    os << "[ I3MCPMTResponse::"
       << "\n  BinSize  :" << r.GetBinSize()
       << "\n  StartTime:" << r.GetStartTime()
       << "\n  EndTime  :" << r.GetEndTime()
       << "\n  Waveform :[";

    const std::vector<double>& w = r.GetWaveform();

    for (std::vector<double>::const_iterator
           iter = w.begin(), end = w.end(); iter != end; ++iter)
        os << *iter << " ";

    os << "] ]";
    return os;
}

I3_SERIALIZABLE(I3MCPMTResponseMap);
