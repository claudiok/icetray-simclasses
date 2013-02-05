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

template <typename TimeType>
bool is_sorted(typename std::vector<std::pair<TimeType,uint32_t> >::const_iterator start,
               typename std::vector<std::pair<TimeType,uint32_t> >::const_iterator finish)
{
  typename std::vector<std::pair<TimeType,uint32_t> >::const_iterator i(start);
  typename std::vector<std::pair<TimeType,uint32_t> >::const_iterator j(i);
  ++j;
  for(; j != finish; ++i,++j)
    if(i->first > j->first) return false;
  return true;  
}

/**
 * Structure that holds MC SPE photocathode arrival times.  
 * This structure supports both binned and unbinned (and even
 * mixed binning) hits.  The bin width determines whether
 * it's binned or not.  Bin widths greater than 0 implies
 * binned hits.  Bin widths <=0, nan, or inf imply unbinned.
 */
template <typename TimeType = float> class mcspe_series : 
  public std::vector<std::pair<TimeType,uint32_t> > 
{
  
public:

  typedef std::vector<std::pair<TimeType,uint32_t> > base_type;
  typedef uint32_t npe_type;
  typedef TimeType time_type;

  /**
   * Nullary constructor defaults to unbinned.
   */
  mcspe_series() : 
    bin_width_(0)
  {};

  /**
   * Range constructor needed for dataclasses suite.
   */
  mcspe_series(typename base_type::iterator i, 
	       typename base_type::iterator j) : 
    bin_width_(0)
  { 
    std::copy(i,j, std::back_inserter(*this));
  }

  /**
   * Get/Setter for bin width.  The Getter is trivial.  The
   * Setter however keeps track of switches from unbinned -> binned.
   * It's important that the hits be sorted when binning, so
   * we sort on every transition.
   */ 
  TimeType get_bin_width(){ return bin_width_; }
  void set_bin_width(TimeType b){ 
    bin_width_ = b; 
    if(is_binned() && !this->empty()       
       && !is_sorted<TimeType>(this->begin(),this->end())){
      // transition from possibly unsorted to binned
      // need to sort
      std::sort(this->begin(), this->end(), compare_bin_value_pair<TimeType>);
      return;
    }    
  }

  /**
   * Methods to return npe and arrival_times.  These
   * vectors are guaranteed to be the same size.
   */
  std::vector<npe_type> npe_values();
  std::vector<TimeType> arrival_times();

  /**
   * Convenience method that returns true if this series is binned.
   */
  bool is_binned(){ return std::isnormal(bin_width_) && bin_width_ > 0;}

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

  SET_LOGGER("I3MCSPESeries");

private:  
  TimeType bin_width_;
  bool is_sorted_;

  friend class boost::serialization::access;
  template <class Archive> void serialize(Archive & ar, const unsigned version)
  {    
    ar & make_nvp("base", base_object<base_type>(*this));
    ar & make_nvp("bin_width",bin_width_);
    ar & make_nvp("is_sorted",is_sorted_);
  }

};

BOOST_CLASS_VERSION(mcspe_series<>,i3mcspeseries_version_);

/**
 * The 'bracket' function searches the container 'cont' for two
 * elements that bound the given 'value' and returns their iterators.
 * This makes no attempt at range-checking so is the responsibilty
 * of the caller to make sure 'value' is between cont.front() and
 * cont.back().
 */
template <typename TimeType>
std::pair<typename mcspe_series<TimeType>::iterator,
          typename mcspe_series<TimeType>::iterator>
bracket(typename mcspe_series<TimeType>::base_type& cont, TimeType value){
 
  // use std::upper_bound to get the first element in the vector that compares 
  // greater than 'value.'  std::lower_bound would work just the same, but 
  // would make less sense.  it returns the first element that does not compare 
  // less than 'value.'  these make more sense when using them to search
  // containers with identical (e.g. see std::equal_range)

  // the 'npe' is meaningless here since we only compare 'time-like' values
  typename mcspe_series<TimeType>::value_type search_value(value,1);
  // iterator to the element that compares greater than 'value'
  typename mcspe_series<TimeType>::iterator upper = 
    std::upper_bound(cont.begin(), cont.end(), 
                     search_value, compare_bin_value_pair<TimeType>);

  // copy 'upper' and decrement gets the element that compares less than 'value'
  typename mcspe_series<TimeType>::iterator lower(upper);
  --lower;

  return 
    std::pair<typename mcspe_series<TimeType>::iterator,
              typename mcspe_series<TimeType>::iterator> 
  (lower,upper);    
}


/**
 * This is the 'fill' method that is called by all other filled methods.
 * It first checks whether this series is binned or not.  If it's unbinned
 * just push_back and return.  If it's binned check if it's out-of-range,
 * in which you're either going insert in front, or push_back.
 */
template <typename TimeType>
void mcspe_series<TimeType>::fill(TimeType b, npe_type w){

  if( !is_binned() || this->empty()){  
    this->push_back(typename base_type::value_type(b,w));
    return;
  }

  if(b < this->front().first){
    // create a new bin and insert at the front
    TimeType distance(fabs(this->front().first - b));    
    int32_t nbins(1 + distance/bin_width_);
    TimeType offset(nbins * bin_width_);
    TimeType le(this->front().first - offset);
    this->insert(this->begin(), typename base_type::value_type(le,w));
    return;
  }

  if(b > this->back().first + bin_width_){
    // create and new bin and push_back
    TimeType distance(b - this->back().first);    
    int32_t nbins(distance/bin_width_);
    TimeType offset(nbins * bin_width_);
    TimeType le(this->back().first + offset);
    this->push_back(typename base_type::value_type(le,w));
    return;
  }

  // we've checked whether it's before the front bin
  // and after the back bin.  if we've gotten this far
  // it's somewhere in between.
  std::pair<typename mcspe_series<TimeType>::iterator,
            typename mcspe_series<TimeType>::iterator>
    low_high_pair = bracket<TimeType>(*this,b);

  // it's ok if low_high_pair.second is end()
  // we never dereference it.  it's only used for insertion.
  assert(low_high_pair.first != this->end());

  TimeType distance(b - low_high_pair.first->first);

  if( distance < bin_width_ ){
    // 'value' is in the low bin. just increment 'npe' 
    low_high_pair.first->second += w; 
  }else{
    // calculate an integral number of bins from the 'low' bin.
    // create a new bin and insert it before the 'high' bin.
    // again 'high' bin iterator == end() is perfectly valid.
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
