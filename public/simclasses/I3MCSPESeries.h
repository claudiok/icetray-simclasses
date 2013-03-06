/**
    copyright  (C) 2013
    the icecube collaboration
    @file I3MCSPESeries.h
    @version $Id$
    @date $Date$
*/

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

template <typename TimeType>
bool compare_bin_value_pair(std::pair<TimeType,uint32_t> val,
                            std::pair<TimeType,uint32_t> iter)
{
  return val.first < iter.first;
}

/**
 * Structure that holds MC SPE photocathode arrival times.  
 * This structure supports both binned and unbinned hits.  
 * The bin width determines whether it's binned or not.  
 * Bin widths greater than 0 implies binned hits.  Bin 
 * widths <=0, nan, or inf imply unbinned.
 */
template <typename TimeType = float> 
class mcspe_series 
{
  
public:

  typedef uint32_t npe_type;
  typedef TimeType time_type;
  typedef std::vector<std::pair<time_type, npe_type> > container_type;
  typedef typename container_type::value_type value_type;
  typedef typename container_type::allocator_type allocator_type;
  typedef typename container_type::const_reference const_reference;   
  typedef typename container_type::const_pointer const_pointer;   
  typedef typename container_type::const_iterator const_iterator;   
  typedef typename container_type::const_reverse_iterator const_reverse_iterator;   
  typedef typename container_type::difference_type difference_type;   
  typedef typename container_type::size_type size_type;

  /**
   * Unbinned constructor.
   */
  mcspe_series() : bin_width_(0) {};
  /**
   * This is the only way to set the bin width.  mcspe_series are
   * set to be binned or unbinned at construction only.
   */
  mcspe_series(time_type bin_width) : bin_width_(bin_width) {};

  void sort(){
    if(!is_binned())
      std::sort(values_.begin(), values_.end(), compare_bin_value_pair<time_type>);
  }
  /**
   * Only a getter for bin_width in this version.  
   */ 
  TimeType get_bin_width() const { return bin_width_; }

  size_t size() const { return values_.size(); }
  const_iterator cbegin() const { return values_.begin(); } // nod to c++11
  const_iterator cend() const { return values_.end(); } // nod to c++11
  const_reverse_iterator crbegin() const { return values_.rbegin(); } // nod to c++11
  const_reverse_iterator crend() const { return values_.rend(); } // nod to c++11
  size_t max_size() const { return values_.max_size(); }
  size_t capacity() const { return values_.capacity(); }
  size_t empty() const { return values_.empty(); }
  const_reference operator[](size_type n) const { return values_[n]; }
  const_reference at(size_type n) const { return values_.at(n); }
  const_reference front() const { return values_.front(); }
  const_reference back() const { return values_.back(); }
  allocator_type get_allocator() const { return values_.get_allocator(); }

  /**
   * Methods to return npe and arrival_times.  These
   * vectors are guaranteed to be the same size.
   */
  std::vector<npe_type> npe_values() const;
  std::vector<TimeType> arrival_times() const;

  /**
   * Convenience method that returns true if this series is binned.
   */
  bool is_binned() const { return std::isnormal(bin_width_) && bin_width_ > 0;}

  /**  
   * Various fill methods.  The first three are implemented for convenience
   * and call the last one (i.e. fill(TimeType,npe_type)), which does all 
   * the heavy lifting.
   *
   * If this series is unbinned 'fill' simply creates a pair, pushes it back,
   * and returns.  For binned cases it searches for two elements that 
   * straddle the given time and either creates and new bin or increments
   * an existing bin.
   */
  void fill(const std::vector<TimeType>& times){
    BOOST_FOREACH(typename std::vector<TimeType>::const_reference t,times)
      fill(t);
  };
  void fill(const std::vector<TimeType>& times, const std::vector<npe_type>& npes){
    if( times.size() != npes.size()){
      log_error("time(%zu) and npe(%zu) size must be the same.",
                times.size(),npes.size());
      return;
    }
    typename std::vector<TimeType>::const_iterator t_iter = times.begin();
    typename std::vector<npe_type>::const_iterator n_iter = npes.begin();
    for(; t_iter != times.end() && n_iter != npes.end(); ++t_iter, ++n_iter)
      fill(*t_iter,*n_iter);
  };
  void fill(TimeType time){ fill(time,1); };
  void fill(TimeType time, npe_type npe); 

  SET_LOGGER("I3MCSPESeries");

private:  
  TimeType bin_width_;
  container_type values_;

  typedef typename container_type::reference reference;   
  typedef typename container_type::pointer pointer;   
  typedef typename container_type::iterator iterator;   
  typedef typename container_type::reverse_iterator reverse_iterator;   

  friend class boost::serialization::access;
  template <class Archive> void serialize(Archive & ar, const unsigned version)
  {    
    ar & make_nvp("values", values_);
    ar & make_nvp("bin_width", bin_width_);
  }

};

BOOST_CLASS_VERSION(mcspe_series<>,i3mcspeseries_version_);

/**
 * This is the 'fill' method that is called by all other filled methods.
 * It first checks whether this series is binned or not.  If it's unbinned
 * just push_back and return.  If it's binned check if it's out-of-range,
 * in which you're either going insert in front, or push_back.
 */
template <typename TimeType>
void mcspe_series<TimeType>::fill(TimeType time, npe_type npe){

  if( !is_binned() || values_.empty()){  
    values_.push_back(value_type(time,npe));
    return;
  }

  if(time < values_.front().first){
    // create a new bin and insert at the front
    TimeType distance(fabs(values_.front().first - time));    
    int32_t nbins(1 + distance/bin_width_);
    TimeType offset(nbins * bin_width_);
    TimeType le(values_.front().first - offset); // leading edge
    values_.insert(values_.begin(), value_type(le,npe));
    return;
  }

  if(time > values_.back().first + bin_width_){
    // create and new bin and push_back
    TimeType distance(time - values_.back().first);    
    int32_t nbins(distance/bin_width_);
    TimeType offset(nbins * bin_width_);
    TimeType le(values_.back().first + offset); // leading edge
    values_.push_back(value_type(le,npe));
    return;
  }

  // use std::upper_bound to get the first element in the vector that compares 
  // greater than 'value.'  std::lower_bound would work just the same, but 
  // would make less sense.  it returns the first element that does not compare 
  // less than 'value.'  these make more sense when using them to search
  // containers with identical (e.g. see std::equal_range)

  // the 'npe' is meaningless here since we only compare 'time-like' values
  value_type search_value(time,1);
  // iterator to the element that compares greater than 'value'
  iterator upper = std::upper_bound(values_.begin(), values_.end(),     
                                    search_value, compare_bin_value_pair<TimeType>);

  // copy 'upper' and decrement gets the element that compares less than 'value'
  iterator lower(upper);
  --lower;

  // don't care if upper is end. 
  // we never dereference it.  
  // it's only used for insertion.
  assert(lower != values_.end());

  TimeType distance(time - lower->first);

  if( distance < bin_width_ ){
    // 'value' is in the low bin. just increment 'npe' 
    lower->second += npe; 
  }else{
    // calculate an integral number of bins from the 'low' bin.
    // create a new bin and insert it before the 'high' bin.
    // again 'high' bin iterator == end() is perfectly valid.
    int32_t nbins(distance/bin_width_);
    TimeType offset(nbins * bin_width_);
    TimeType le(lower->first + offset);
    values_.insert(upper, value_type(le,npe));
  }
}

template <typename TimeType>
std::vector<TimeType> mcspe_series<TimeType>::arrival_times() const {
  std::vector<TimeType> times;
  BOOST_FOREACH(const_reference v_pair, values_)
    times.push_back(v_pair.first);
  return times;
}

template <typename TimeType>
std::vector<typename mcspe_series<TimeType>::npe_type> 
mcspe_series<TimeType>::npe_values() const {
  std::vector<typename mcspe_series<TimeType>::npe_type> npes;
  BOOST_FOREACH(const_reference v_pair, values_)
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
