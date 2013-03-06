/**
 * copyright  (C) 2004
 * the icecube collaboration
 * @version $Id: ParticleIDKey.h 94851 2012-11-01 18:46:55Z nega $
 * @file ParticleIDKey.h
 * @date $Date: 2012-11-01 12:46:55 -0600 (Thu, 01 Nov 2012) $
 */

#ifndef PARTICLEIDKEY_H_INCLUDED
#define PARTICLEIDKEY_H_INCLUDED

/**
 * @brief ParticleIDKey class 
 * 
 */

#include <icetray/serialization.h>

class I3Particle;
struct ParticleIDKey{

  int minor_ID;
  uint64_t major_ID;

  ParticleIDKey(const I3Particle& p);
  ParticleIDKey(){}; //remove me

  bool operator==(const ParticleIDKey& rhs) {
    return major_ID == rhs.major_ID
    && minor_ID == rhs.minor_ID;
  }
  
 private:
  friend class boost::serialization::access;
  template <class Archive> void serialize(Archive & ar, unsigned version);
  
};

inline bool operator<(const ParticleIDKey& lhs,const ParticleIDKey& rhs)
{
  if(lhs.major_ID != rhs.major_ID)
    return lhs.major_ID < rhs.major_ID;
  else return lhs.minor_ID < rhs.minor_ID;
}

#endif //PARTICLEIDKEY_H_INCLUDED

