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

template <typename TimeType = float> class mcspe_series : 
  public std::vector<std::pair<TimeType,uint32_t> > 
{
  
public:

  typedef std::vector<std::pair<TimeType,uint32_t> > base_type;
  typedef uint32_t npe_type;
  typedef TimeType time_type;

  mcspe_series() : bin_width_(0.){};
  mcspe_series(TimeType bin_width) : bin_width_(bin_width){};
  mcspe_series(typename base_type::iterator i, 
	       typename base_type::iterator j) : 
    bin_width_(0.)
  { 
    std::copy(i,j, std::back_inserter(*this));
  }

  TimeType get_bin_width(){ return bin_width_; }
  TimeType set_bin_width(TimeType b){ bin_width_ = b; }
  
  void fill(const std::vector<TimeType>& v){
    BOOST_FOREACH(typename std::vector<TimeType>::const_reference r,v)
    fill(r);
  };
  void fill(const std::vector<TimeType>& v, const std::vector<npe_type>& n){
    if( v.size() != n.size()){
      log_error("time(%zu) and npe(%zu) size must be the same.",
                v.size(),n.size());
      return;
    }
    typename std::vector<TimeType>::const_iterator t_iter = v.begin();
    typename std::vector<npe_type>::const_iterator n_iter = n.begin();
    for(; t_iter != v.end() && n_iter != n.end(); ++t_iter, ++n_iter)
      fill(*t_iter,*n_iter);
  };
  void fill(TimeType b){ fill(b,1); };
  void fill(TimeType b, npe_type w); 

  std::vector<npe_type> npe_values();
  std::vector<TimeType> arrival_times();

  bool is_binned(){ return std::isnormal(bin_width_) && bin_width_ > 0;}

  SET_LOGGER("I3MCSPESeries");

private:  
  TimeType bin_width_;

  friend class boost::serialization::access;
  template <class Archive> void serialize(Archive & ar, const unsigned version)
  {    
    ar & make_nvp("base", base_object<base_type>(*this));
    ar & make_nvp("bin_width",bin_width_);
  }

};

BOOST_CLASS_VERSION(mcspe_series<>,i3mcspeseries_version_);

template <typename TimeType>
bool compare_bin_value_pair(typename mcspe_series<TimeType>::value_type val,
                            typename mcspe_series<TimeType>::value_type iter)
{
  return val.first < iter.first;
}

template <typename TimeType>
std::pair<typename mcspe_series<TimeType>::iterator,
          typename mcspe_series<TimeType>::iterator>
bracket(typename mcspe_series<TimeType>::base_type& cont, TimeType value){

  if(value < cont.front().first || value > cont.back().first ){     
    return 
      std::pair<typename mcspe_series<TimeType>::iterator,
                typename mcspe_series<TimeType>::iterator> 
      (cont.end(),cont.end());    
  }

  if( cont.size() == 1 ){
    return 
      std::pair<typename mcspe_series<TimeType>::iterator,
                typename mcspe_series<TimeType>::iterator> 
      (cont.begin(),cont.end());
  }

  typename mcspe_series<TimeType>::value_type search_value(value,1);
  typename mcspe_series<TimeType>::iterator upper = 
    std::upper_bound(cont.begin(), cont.end(), 
                     search_value, compare_bin_value_pair<TimeType>);

  typename mcspe_series<TimeType>::iterator lower(upper);
  --lower;

  return 
    std::pair<typename mcspe_series<TimeType>::iterator,
              typename mcspe_series<TimeType>::iterator> 
  (lower,upper);    
}


template <typename TimeType>
void mcspe_series<TimeType>::fill(TimeType b, npe_type w){

  if(!std::isnormal(bin_width_) || bin_width_ < 0. || this->empty()){  
    this->push_back(typename base_type::value_type(b,w));
    return;
  }

  if(b < this->front().first){
    TimeType distance(fabs(this->front().first - b));    
    int32_t nbins(1 + distance/bin_width_);
    TimeType offset(nbins * bin_width_);
    TimeType le(this->front().first - offset);
    this->insert(this->begin(), typename base_type::value_type(le,w));
    return;
  }

  if(b >= this->back().first){
    if( b < this->back().first + bin_width_ ){
      this->at( this->size() - 1).second += w;
    }else{
      TimeType distance(b - this->back().first);    
      int32_t nbins(distance/bin_width_);
      TimeType offset(nbins * bin_width_);
      TimeType le(this->back().first + offset);
      this->push_back(typename base_type::value_type(le,w));
    }
    return;
  }

  // we've checked whether it's before the front bin
  // and after the back bin.  if we've gotten this far
  // it's somewhere in between.
  std::pair<typename mcspe_series<TimeType>::iterator,
            typename mcspe_series<TimeType>::iterator>
    low_high_pair = bracket<TimeType>(*this,b);

  TimeType distance(b - low_high_pair.first->first);

  if( distance < bin_width_ ){
    low_high_pair.first->second += w; 
  }else{
    // calculate an integral number of bins
    int32_t nbins(distance/bin_width_);
    TimeType offset(nbins * bin_width_);
    TimeType le(low_high_pair.first->first + offset);
    this->insert(low_high_pair.second, typename base_type::value_type(le,w));
  }
}

template <typename TimeType>
std::vector<TimeType> mcspe_series<TimeType>::arrival_times(){
  std::vector<TimeType> times;
  BOOST_FOREACH(typename base_type::value_type& v_pair, *this)
    times.push_back(v_pair.first);
  return times;
}

template <typename TimeType>
std::vector<typename mcspe_series<TimeType>::npe_type> mcspe_series<TimeType>::npe_values(){
  std::vector<typename mcspe_series<TimeType>::npe_type> npes;
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
