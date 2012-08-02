#include <icetray/serialization.h>
#include <icetray/I3Units.h>
#include <simclasses/I3MCPMTResponse.h>
#include <boost/foreach.hpp>

template <class Archive>
    void I3MCPMTResponse::save(Archive& ar, unsigned version) const {
  ar & make_nvp("CompressedWaveform",compressed_waveform_);
  ar & make_nvp("BinSize",binSize_);
  ar & make_nvp("startTime",startTime_);
  ar & make_nvp("endTime",endTime_);
}

template <class Archive>
    void I3MCPMTResponse::load(Archive& ar, unsigned version){
  if(version == 9999){
    ar & make_nvp("CompressedWaveform",compressed_waveform_);
  }else{
    std::vector<double> wf;
    ar & make_nvp("Waveform",wf);
    Compress(wf);
  }
  ar & make_nvp("BinSize",binSize_);
  ar & make_nvp("startTime",startTime_);
  ar & make_nvp("endTime",endTime_);
}

I3_SERIALIZABLE(I3MCPMTResponseMap);

void I3MCPMTResponse::Compress(const std::vector<double>& wf){

  compressed_waveform_.clear();
  double cv(wf.front());//current value
  //double tolerance(1e-6 * I3Units::mV);
  int nbins(1);//there's always at least one bin
  BOOST_FOREACH(const double& v, wf){
    if(fabs(v - cv) < tolerance_){
      nbins++;
    }else{
      compressed_wf_t::value_type e(cv,nbins);
      compressed_waveform_.push_back(e);
      //reset the current nbins and cv
      nbins = 1;
      cv = v;
    }
  }  
}

std::vector<double> 
I3MCPMTResponse::Decompress() const {
  std::vector<double> decompressed_wf;
  BOOST_FOREACH(const compressed_wf_t::value_type& e, compressed_waveform_)	   
    for(int i(0); i<e.first; i++)
      decompressed_wf.push_back(e.second);
  return decompressed_wf;
}

