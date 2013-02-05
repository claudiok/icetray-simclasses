/**
    copyright  (C) 2004
    the icecube collaboration
    $Id: I3MCSPEHistogramTest.cxx 86642 2012-03-28 06:03:57Z olivas $

    @version $Revision: 1.8 $
    @date $Date: 2012-03-28 00:03:57 -0600 (Wed, 28 Mar 2012) $
    @author Alex Olivas (olivas@icecube.umd.edu)
*/

#include <vector>
#include <iostream>
#include <I3Test.h>
#include <boost/assign/list_of.hpp>
#include <boost/random.hpp>
#include <simclasses/I3MCSPESeries.h>

using boost::assign::list_of;

TEST_GROUP(I3MCSPESeriesTest);

TEST(instantiation)
{
  I3MCSPESeries h_float;  // default is 'float'
  I3MCSPEHistogram<double> h_double;
  I3MCSPEHistogram<int> h_int;
  I3MCSPEHistogram<int32_t> hd_int32_t;
  I3MCSPEHistogram<uint32_t> hd_uint32_t;
  I3MCSPEHistogram<short> h_short;
};

TEST(unbinned){
  I3MCSPESeries h;  // default is 'float'

  std::vector<float> i = list_of(9.)(9.)(9.);

  h.fill(i);

  std::cerr<<std::endl;
  std::cerr<<"h.npe_values().size() = "<<h.npe_values().size()<<std::endl;
  std::cerr<<"h.arrival_times().size() = "<<h.arrival_times().size()<<std::endl;

  ENSURE(h.npe_values().size() == 3);
  ENSURE(h.arrival_times().size() == 3);
};

TEST(binned_simple){
  I3MCSPESeries h(1.0);  // default is 'float'

  std::vector<float> i = list_of(9.)(9.)(9.);

  h.fill(i);

  std::cerr<<std::endl;
  std::cerr<<"h.is_binned() = "<<h.is_binned()<<std::endl;
  std::cerr<<"h.get_bin_width() = "<<h.get_bin_width()<<std::endl;
  std::cerr<<"h.npe_values().size() = "<<h.npe_values().size()<<std::endl;
  std::cerr<<"h.arrival_times().size() = "<<h.arrival_times().size()<<std::endl;

  ENSURE(h.npe_values().size() == 1);
  ENSURE(h.arrival_times().size() == h.npe_values().size());
};

TEST(binned_less_simple){
  I3MCSPESeries h(1.0);  // default is 'float'

  std::vector<float> i = list_of(1.)(9.)(9.)(9.)(10.)(20.);

  h.fill(i);

  std::cerr<<std::endl;
  std::cerr<<"h.is_binned() = "<<h.is_binned()<<std::endl;
  std::cerr<<"h.get_bin_width() = "<<h.get_bin_width()<<std::endl;
  std::cerr<<"h.npe_values().size() = "<<h.npe_values().size()<<std::endl;
  std::cerr<<"h.arrival_times().size() = "<<h.arrival_times().size()<<std::endl;

  ENSURE(h.npe_values().size() == 4);
  ENSURE(h.arrival_times().size() == h.npe_values().size());

  BOOST_FOREACH(int32_t bv, h.npe_values()){
    std::cerr<<bv<<" ";
  }
  std::cerr<<std::endl;

  BOOST_FOREACH(int32_t le, h.arrival_times()){
    std::cerr<<le<<" ";
  }
  std::cerr<<std::endl;

  for(size_t i(0); i<h.npe_values().size(); ++i)
    std::cerr<<h.npe_values()[i]<<" ";    
  std::cerr<<std::endl;

  ENSURE(h.npe_values()[0] == 1);
  ENSURE(h.npe_values()[1] == 3);
  ENSURE(h.npe_values()[2] == 1);
  ENSURE(h.npe_values()[3] == 1);
};

TEST(weighted_binned){
  I3MCSPESeries h(1.0);  // default is 'float'

  std::vector<float> t = list_of(1.)(9.)(9.)(9.)(10.)(20.);
  std::vector<int32_t> w = list_of(6)(5)(4)(3)(2)(1);

  for(int i(0); i < t.size(); ++i)
    h.fill(t[i],w[i]);

  std::cerr<<std::endl;
  std::cerr<<"h.is_binned() = "<<h.is_binned()<<std::endl;
  std::cerr<<"h.get_bin_width() = "<<h.get_bin_width()<<std::endl;
  std::cerr<<"h.npe_values().size() = "<<h.npe_values().size()<<std::endl;
  std::cerr<<"h.arrival_times().size() = "<<h.arrival_times().size()<<std::endl;

  ENSURE(h.npe_values().size() == 4);
  ENSURE(h.arrival_times().size() == h.npe_values().size());

  BOOST_FOREACH(int32_t bv, h.npe_values()){
    std::cerr<<bv<<" ";
  }
  std::cerr<<std::endl;

  BOOST_FOREACH(int32_t le, h.arrival_times()){
    std::cerr<<le<<" ";
  }
  std::cerr<<std::endl;

  for(size_t i(0); i<h.npe_values().size(); ++i)
    std::cerr<<h.npe_values()[i]<<" ";    
  std::cerr<<std::endl;

  ENSURE(h.npe_values()[0] == 6);
  ENSURE(h.npe_values()[1] == 12);
  ENSURE(h.npe_values()[2] == 2);
  ENSURE(h.npe_values()[3] == 1);
};

TEST(binned_stress_test){
  I3MCSPESeries h(1.0);  // default is 'float'

  float nbins(1000.);
  boost::mt19937 rng(42u);
  boost::uniform_real<> distribution(0.,1000.);
  boost::variate_generator<boost::mt19937&, boost::uniform_real<> > 
    generator(rng,distribution);

  int N(100000000);
  for(int i(0); i < N; ++i){
    h.fill(generator());
  }

  std::cerr<<std::endl;
  std::cerr<<"expected unbinned memory size = " << N * 8.0 * 1e-6 <<" MB"<<std::endl;
  std::cerr<<"expected binned memory size = " << nbins * 8.0 * 1e-6 <<" MB"<<std::endl;
  std::cerr<<"h.is_binned() = "<<h.is_binned()<<std::endl;
  std::cerr<<"h.get_bin_width() = "<<h.get_bin_width()<<std::endl;
  std::cerr<<"h.npe_values().size() = "<<h.npe_values().size()<<std::endl;
  std::cerr<<"h.arrival_times().size() = "<<h.arrival_times().size()<<std::endl;

};

TEST(binned_stress_test_vector){
  I3MCSPESeries h(1.0);  // default is 'float'

  boost::mt19937 rng(42u);
  boost::uniform_real<> distribution(0.,1000.);
  boost::variate_generator<boost::mt19937&, boost::uniform_real<> > 
    generator(rng,distribution);

  int N(100000000);
  std::vector<float> values;
  for(int i(0); i < N; ++i){
    values.push_back(generator());
  }

  std::sort(values.begin(),values.end());
  h.fill(values);

  std::cerr<<std::endl;
  std::cerr<<"h.is_binned() = "<<h.is_binned()<<std::endl;
  std::cerr<<"h.get_bin_width() = "<<h.get_bin_width()<<std::endl;
  std::cerr<<"h.npe_values().size() = "<<h.npe_values().size()<<std::endl;
  std::cerr<<"h.arrival_times().size() = "<<h.arrival_times().size()<<std::endl;

  for(size_t i(0); i < h.npe_values().size(); ++i){
    std::cerr<<h.arrival_times()[i]<<" : "<<h.npe_values()[i]<<std::endl;             
  }

};

