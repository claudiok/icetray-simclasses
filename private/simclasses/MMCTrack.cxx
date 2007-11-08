#include <icetray/serialization.h>
#include <dataclasses/physics/I3Particle.h>

/***
 * WARNING!!! This class only exists to maintain backwards compatibility.
 * This allows for the reading of old 1.9.6 (and older) era data.
 */

class MMCTrack 
{
 private:
  I3Particle particle_;
  double Elost;
  // coordinates at entry point [m,m,m,sec,GeV]
  double xi, yi, zi, ti, Ei;

  // coordinates at exit point
  double xf, yf, zf, tf, Ef;

  // coordinates at point of closest approach
  double xc, yc, zc, tc, Ec;

  friend class boost::serialization::access;
  template <class Archive> void serialize(Archive& ar, unsigned version);
};

template <class Archive> 
void MMCTrack::serialize(Archive& ar, unsigned version)
{
  ar & make_nvp("I3Particle", particle_);
  ar & make_nvp("xi", xi);
  ar & make_nvp("yi", yi);
  ar & make_nvp("zi", zi);
  ar & make_nvp("ti", ti);
  ar & make_nvp("Ei", Ei);
  ar & make_nvp("xf", xf);
  ar & make_nvp("yf", yf);
  ar & make_nvp("zf", zf);
  ar & make_nvp("tf", tf);
  ar & make_nvp("Ef", Ef);
  ar & make_nvp("xc", xc);
  ar & make_nvp("yc", yc);
  ar & make_nvp("zc", zc);
  ar & make_nvp("tc", tc);
  ar & make_nvp("Ec", Ec);
  ar & make_nvp("Elost", Elost);
}

#define I3_SERIALIZABLE_DONT_TRY_THIS_AT_HOME(T,N)						\
  template void T::serialize(boost::archive::portable_binary_oarchive&, unsigned); \
  template void T::serialize(boost::archive::portable_binary_iarchive&, unsigned); \
  template void T::serialize(boost::archive::xml_oarchive&, unsigned);	\
  template void T::serialize(boost::archive::xml_iarchive&, unsigned);	\
  TTREE_OARCHIVE_INSTANTIATION(T)					\
  template std::string AsXML(const T&);					\
  template std::string AsXML(boost::shared_ptr<T> const&);		\
  template std::string AsXML(boost::shared_ptr<const T> const&);	\
  BOOST_CLASS_EXPORT_GUID(T,N);					
                                                                                  
I3_SERIALIZABLE_DONT_TRY_THIS_AT_HOME(MMCTrack,"I3MMCTrack");

typedef I3Vector<MMCTrack> MMCTrackList;

I3_SERIALIZABLE(MMCTrackList);
