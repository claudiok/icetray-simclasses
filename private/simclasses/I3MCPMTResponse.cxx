#include <icetray/serialization.h>
#include <icetray/I3Units.h>
#include <simclasses/I3MCPMTResponse.h>
#include <boost/foreach.hpp>

template <class Archive>
    void I3MCPMTResponse::serialize(Archive& ar, unsigned version){
  if(version > 0){
    ar & make_nvp("CompressedWaveform",compressed_waveform_);
    ar & make_nvp("Compressed",compressed_);
  }
  ar & make_nvp("Waveform",waveform_);
  ar & make_nvp("BinSize",binSize_);
  ar & make_nvp("startTime",startTime_);
  ar & make_nvp("endTime",endTime_);
}

I3_SERIALIZABLE(I3MCPMTResponseMap);

typedef std::pair<int,double> comp_el;

void I3MCPMTResponse::Compress(){

  compressed_waveform_.clear();
  double cv(waveform_.front());//current value
  double tolerance(1e-6 * I3Units::mV);
  int nbins(1);//there's always at least one bin
  for_each(const double& v, waveform_){
    //flat waveforms will be lost
    if(fabs(v - cv) < tolerance){
      nbins++;
    }else{
      std::pair<int,double> e(nbins,cv);
      compressed_waveform_.push_back(e);
      //reset the current nbins and cv
      nbins = 1;
      cv = v;
    }
  }  
  waveform_.clear();
  compressed_=true;
}

void I3MCPMTResponse::Decompress(){
  waveform_.clear();
  int counter(0);
  for_each(const comp_el& e, compressed_waveform_){
    for(int i(0); i<e.first; i++){
      waveform_.push_back(e.second);
    }
  }
}

