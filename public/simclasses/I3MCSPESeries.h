#ifndef I3MCSPESERIES_H_INCLUDED
#define I3MCSPESERIES_H_INCLUDED

#include <algorithm>
#include <vector>
#include <boost/foreach.hpp>
#include <icetray/I3Logging.h>
#include <icetray/serialization.h>
#include <dataclasses/I3Map.h>
#include <dataclasses/ParticleIDKey.h>

static const unsigned i3mcspehistogram_version_ = 0;

template <typename BinType = float>
class I3MCSPEHistogram {
  
public:

  // <leading edge, N>
  typedef uint32_t npe_type;
  typedef BinType bin_type;
  typedef std::pair<BinType,npe_type> element_type;
  typedef std::vector< element_type > container_type;
  typedef typename container_type::iterator iterator_type;

  I3MCSPEHistogram();
  I3MCSPEHistogram(BinType bin_width);
  I3MCSPEHistogram(BinType bin_width, const std::vector<BinType>& v);

  BinType get_bin_width(){ return bin_width_; }
  const container_type& get_values() const { return values_; }
  
  void fill(BinType b);
  void fill(BinType b, npe_type w); 
  void fill(const std::vector<BinType>& v);

  std::vector<npe_type> bin_values();
  std::vector<BinType> leading_edges();

  bool is_binned(){ return std::isnormal(bin_width_) || bin_width_ < 0;}

  SET_LOGGER("I3MCSPEHistogram");

private:  
  container_type values_;
  BinType bin_width_;

  friend class boost::serialization::access;
  template <class Archive> void serialize(Archive & ar, const unsigned version)
  {
    ar & make_nvp("values",values_);
    ar & make_nvp("bin_width",bin_width_);
  }

};

BOOST_CLASS_VERSION(I3MCSPEHistogram<>,i3mcspehistogram_version_);

/**
 * SOURCE CODE BELOW
 */ 
template <typename BinType>
I3MCSPEHistogram<BinType>::I3MCSPEHistogram(BinType width) :
  bin_width_(width){};

template <typename BinType>
I3MCSPEHistogram<BinType>::I3MCSPEHistogram() :
  bin_width_(0.){};

template <typename BinType>
I3MCSPEHistogram<BinType>::
I3MCSPEHistogram(BinType width, const std::vector<BinType>& v):
  bin_width_(width)
{ 
  BOOST_FOREACH(typename std::vector<BinType>::const_reference r,v)
    fill(r);
}

template <typename BinType>
bool compare_bin_value_pair(typename I3MCSPEHistogram<BinType>::element_type val,
                            typename I3MCSPEHistogram<BinType>::element_type iter)
{
  return val.first < iter.first;
}

template <typename BinType>
std::pair<typename I3MCSPEHistogram<BinType>::container_type::iterator,
          typename I3MCSPEHistogram<BinType>::container_type::iterator>
bracket(typename I3MCSPEHistogram<BinType>::container_type& cont, BinType value){

  if(value < cont.front().first || value > cont.back().first ){     
    return 
      std::pair<typename I3MCSPEHistogram<BinType>::container_type::iterator,
                typename I3MCSPEHistogram<BinType>::container_type::iterator> 
      (cont.end(),cont.end());    
  }

  if( cont.size() == 1 ){
    return 
      std::pair<typename I3MCSPEHistogram<BinType>::container_type::iterator,
                typename I3MCSPEHistogram<BinType>::container_type::iterator> 
      (cont.begin(),cont.end());
  }

  typename I3MCSPEHistogram<BinType>::element_type search_value(value,1);
  typename I3MCSPEHistogram<BinType>::container_type::iterator upper = 
    std::upper_bound(cont.begin(), cont.end(), 
                     search_value, compare_bin_value_pair<BinType>);                     

  typename I3MCSPEHistogram<BinType>::container_type::iterator lower(upper);
  --lower;

  return 
    std::pair<typename I3MCSPEHistogram<BinType>::container_type::iterator,
              typename I3MCSPEHistogram<BinType>::container_type::iterator> 
  (lower,upper);    
}

template <typename BinType>
void I3MCSPEHistogram<BinType>::fill(const std::vector<BinType>& v){  
  BOOST_FOREACH(typename std::vector<BinType>::const_reference r,v)
    fill(r);
}

template <typename BinType>
void I3MCSPEHistogram<BinType>::fill(BinType b){
  fill(b,1);
}

template <typename BinType>
void I3MCSPEHistogram<BinType>::fill(BinType b, npe_type w){

  if(!std::isnormal(bin_width_) || bin_width_ < 0. || values_.empty()){  
    values_.push_back(element_type(b,w));
    return;
  }

  if(b < values_.front().first){
    BinType distance(fabs(values_.front().first - b));    
    int32_t nbins(1 + distance/bin_width_);
    BinType offset(nbins * bin_width_);
    BinType le(values_.front().first - offset);
    values_.insert(values_.begin(), element_type(le,w));
    return;
  }

  if(b >= values_.back().first){
    if( b < values_.back().first + bin_width_ ){
      values_.at( values_.size() - 1).second += w;
    }else{
      BinType distance(b - values_.back().first);    
      int32_t nbins(distance/bin_width_);
      BinType offset(nbins * bin_width_);
      BinType le(values_.back().first + offset);
      values_.push_back(element_type(le,w));
    }
    return;
  }

  // we've checked whether it's before the front bin
  // and after the back bin.  if we've gotten this far
  // it's somewhere in between.
  std::pair<typename I3MCSPEHistogram<BinType>::container_type::iterator,
            typename I3MCSPEHistogram<BinType>::container_type::iterator>
    low_high_pair = bracket<BinType>(values_,b);

  BinType distance(b - low_high_pair.first->first);

  if( distance < bin_width_ ){
    low_high_pair.first->second += w; 
  }else{
    // calculate an integral number of bins
    int32_t nbins(distance/bin_width_);
    BinType offset(nbins * bin_width_);
    BinType le(low_high_pair.first->first + offset);
    values_.insert(low_high_pair.second, element_type(le,w));
  }
}

template <typename BinType>
std::vector<BinType> I3MCSPEHistogram<BinType>::leading_edges(){
  std::vector<BinType> le_values;
  BOOST_FOREACH(typename container_type::value_type& v_pair, values_){
    le_values.push_back(v_pair.first);
  }
  return le_values;
}

template <typename BinType>
std::vector<typename I3MCSPEHistogram<BinType>::npe_type> I3MCSPEHistogram<BinType>::bin_values(){
  std::vector<typename I3MCSPEHistogram<BinType>::npe_type> b_values;
  BOOST_FOREACH(typename container_type::value_type& v_pair, values_){
    b_values.push_back(v_pair.second);
  }
  return b_values;
}

// this is going to cause funky shifts on the order of a fraction
// of a bin width.  may need to come back to this.
template <typename BinType>
void operator+=(I3MCSPEHistogram<BinType>& lhs, const I3MCSPEHistogram<BinType>& rhs){
  BOOST_FOREACH(const typename I3MCSPEHistogram<BinType>::container_type::value_type& v_pair, 
                rhs.get_values())
    lhs.fill(v_pair.first,v_pair.second);
}

class ParticleIDKey;

typedef I3MCSPEHistogram<> I3MCSPESeries;
typedef std::map<ParticleIDKey,I3MCSPESeries > I3MCSPESeriesMap;
typedef I3Map<OMKey, std::map<ParticleIDKey,I3MCSPESeries > >  
  I3MCSPESeriesMapMap;
  
I3_POINTER_TYPEDEFS(I3MCSPESeriesMap);
I3_POINTER_TYPEDEFS(I3MCSPESeriesMapMap);

#endif
