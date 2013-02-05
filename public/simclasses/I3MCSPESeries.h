#ifndef I3MCSPESERIES_H_INCLUDED
#define I3MCSPESERIES_H_INCLUDED

#include <algorithm>
#include <vector>
#include <boost/foreach.hpp>
#include <icetray/I3Logging.h>
#include <icetray/serialization.h>
#include <dataclasses/I3Map.h>
#include <simclasses/ParticleIDKey.h>

static const unsigned i3mcspeseries_version_ = 0;

template <typename BinType = float> class mcspe_series : 
  public std::vector<std::pair<BinType,uint32_t> > 
{
  
public:

  typedef std::vector<std::pair<BinType,uint32_t> > base_type;
  typedef uint32_t npe_type;
  typedef BinType time_type;

  mcspe_series() : bin_width_(0.){};
  mcspe_series(BinType bin_width) : bin_width_(bin_width){};
  mcspe_series(BinType bin_width, const std::vector<BinType>& v){
    BOOST_FOREACH(typename std::vector<BinType>::const_reference r,v)
      fill(r);
  }
  mcspe_series(typename base_type::iterator i, 
		   typename base_type::iterator j){
    std::copy(i,j,this->begin());
  }

  BinType get_bin_width(){ return bin_width_; }
  BinType set_bin_width(BinType b){ bin_width_ = b; }
  
  void fill(const std::vector<BinType>& v){
    BOOST_FOREACH(typename std::vector<BinType>::const_reference r,v)
    fill(r);
  };
  void fill(BinType b){ fill(b,1); };
  void fill(BinType b, npe_type w); 

  std::vector<npe_type> npe_values();
  std::vector<BinType> arrival_times();

  bool is_binned(){ return std::isnormal(bin_width_) && bin_width_ > 0;}

  SET_LOGGER("I3MCSPESeries");

private:  
  BinType bin_width_;

  friend class boost::serialization::access;
  template <class Archive> void serialize(Archive & ar, const unsigned version)
  {    
    ar & make_nvp("base", base_object<base_type>(*this));
    ar & make_nvp("bin_width",bin_width_);
  }

};

BOOST_CLASS_VERSION(mcspe_series<>,i3mcspeseries_version_);

template <typename BinType>
bool compare_bin_value_pair(typename mcspe_series<BinType>::value_type val,
                            typename mcspe_series<BinType>::value_type iter)
{
  return val.first < iter.first;
}

template <typename BinType>
std::pair<typename mcspe_series<BinType>::iterator,
          typename mcspe_series<BinType>::iterator>
bracket(typename mcspe_series<BinType>::base_type& cont, BinType value){

  if(value < cont.front().first || value > cont.back().first ){     
    return 
      std::pair<typename mcspe_series<BinType>::iterator,
                typename mcspe_series<BinType>::iterator> 
      (cont.end(),cont.end());    
  }

  if( cont.size() == 1 ){
    return 
      std::pair<typename mcspe_series<BinType>::iterator,
                typename mcspe_series<BinType>::iterator> 
      (cont.begin(),cont.end());
  }

  typename mcspe_series<BinType>::value_type search_value(value,1);
  typename mcspe_series<BinType>::iterator upper = 
    std::upper_bound(cont.begin(), cont.end(), 
                     search_value, compare_bin_value_pair<BinType>);

  typename mcspe_series<BinType>::iterator lower(upper);
  --lower;

  return 
    std::pair<typename mcspe_series<BinType>::iterator,
              typename mcspe_series<BinType>::iterator> 
  (lower,upper);    
}


template <typename BinType>
void mcspe_series<BinType>::fill(BinType b, npe_type w){

  if(!std::isnormal(bin_width_) || bin_width_ < 0. || this->empty()){  
    this->push_back(typename base_type::value_type(b,w));
    return;
  }

  if(b < this->front().first){
    BinType distance(fabs(this->front().first - b));    
    int32_t nbins(1 + distance/bin_width_);
    BinType offset(nbins * bin_width_);
    BinType le(this->front().first - offset);
    this->insert(this->begin(), typename base_type::value_type(le,w));
    return;
  }

  if(b >= this->back().first){
    if( b < this->back().first + bin_width_ ){
      this->at( this->size() - 1).second += w;
    }else{
      BinType distance(b - this->back().first);    
      int32_t nbins(distance/bin_width_);
      BinType offset(nbins * bin_width_);
      BinType le(this->back().first + offset);
      this->push_back(typename base_type::value_type(le,w));
    }
    return;
  }

  // we've checked whether it's before the front bin
  // and after the back bin.  if we've gotten this far
  // it's somewhere in between.
  std::pair<typename mcspe_series<BinType>::iterator,
            typename mcspe_series<BinType>::iterator>
    low_high_pair = bracket<BinType>(*this,b);

  BinType distance(b - low_high_pair.first->first);

  if( distance < bin_width_ ){
    low_high_pair.first->second += w; 
  }else{
    // calculate an integral number of bins
    int32_t nbins(distance/bin_width_);
    BinType offset(nbins * bin_width_);
    BinType le(low_high_pair.first->first + offset);
    this->insert(low_high_pair.second, typename base_type::value_type(le,w));
  }
}

template <typename BinType>
std::vector<BinType> mcspe_series<BinType>::arrival_times(){
  std::vector<BinType> times;
  BOOST_FOREACH(typename base_type::value_type& v_pair, *this)
    times.push_back(v_pair.first);
  return times;
}

template <typename BinType>
std::vector<typename mcspe_series<BinType>::npe_type> mcspe_series<BinType>::npe_values(){
  std::vector<typename mcspe_series<BinType>::npe_type> npes;
  BOOST_FOREACH(typename base_type::value_type& v_pair, *this)
    npes.push_back(v_pair.second);
  return npes;
}

class ParticleIDKey;

typedef mcspe_series<> I3MCSPESeries;
typedef std::map<ParticleIDKey,I3MCSPESeries > I3MCSPESeriesMap;
typedef I3Map<OMKey, std::map<ParticleIDKey,I3MCSPESeries > > I3MCSPESeriesMapMap;
  
I3_POINTER_TYPEDEFS(I3MCSPESeriesMap);
I3_POINTER_TYPEDEFS(I3MCSPESeriesMapMap);

#endif
