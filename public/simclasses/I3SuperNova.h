/**
 * copyright  (C) 2007
 * the IceCube Collaboration
 *
 * @file I3SuperNova.h
 * @version $Revision: 1.1 $
 * @date $Date: 2007/05/15 21:33:43 $
 * @author juancarlos
 *
 */

#ifndef I3SUPERNOVA_H
#define I3SUPERNOVA_H
#include "dataclasses/physics/I3Particle.h"
#include "dataclasses/I3Map.h"
#include "dataclasses/I3Tree.h"
#include "icetray/I3TrayHeaders.h"
#include "dataclasses/I3Direction.h"
#include "simclasses/I3Histogram.h"

class I3SuperNova : public I3FrameObject 
{

 private:

  static int global_minor_id_;

  int ID_;
  int parentID_;

  unsigned int NumberOfNeutrinos_;
  unsigned int NumberOfSecondaries_;
  unsigned int NumberOfPhotons_;

  // coordinates at exit point
  double SNdistance_;
  double InteractionWeight_;
  I3Direction SNdirection_;
  I3Particle::ParticleType NuFlavor_;

  friend class boost::serialization::access;
  template <class Archive> void serialize(Archive& ar, unsigned version);

 public:

  I3SuperNova();
  I3SuperNova(I3Particle::ParticleType flavor);

  int GetID() const { return ID_; }
  int GetParentID() const { return parentID_; }
  // To comply with I3Tree interface
  uint64_t GetMajorID() const { return 0; }


  inline void SetNumberOfNeutrinos(unsigned int nnu) {NumberOfNeutrinos_=nnu;}
  inline unsigned int GetNumberOfNeutrinos() const {return NumberOfNeutrinos_;}

  inline void SetNumberOfSecondaries(unsigned int secondaries) { 
		  NumberOfSecondaries_=secondaries; 
  }
  inline unsigned int GetNumberOfSecondaries() const {return NumberOfSecondaries_;}

  inline void SetNumberOfPhotons(unsigned int ngamma) {NumberOfPhotons_=ngamma;}
  inline unsigned int GetNumberOfPhotons() const {return NumberOfPhotons_;}

  inline void SetSNdistance(double d) {SNdistance_=d;}
  inline double GetSNdistance() const {return SNdistance_;}

  inline void SetSNdirection(I3Direction d) {SNdirection_=d;}
  inline I3Direction& GetSNdirection() {return SNdirection_;}

  inline void SetNuFlavor(I3Particle::ParticleType nf) {NuFlavor_=nf;}
  inline I3Particle::ParticleType GetNuFlavor() const {return NuFlavor_;}

  inline void SetInteractionWeight(double weight) {InteractionWeight_=weight;}
  inline double GetInteractionWeight() {return InteractionWeight_;}

};

I3_POINTER_TYPEDEFS(I3SuperNova);

typedef I3Tree<I3SuperNova> I3SuperNovaTree;
I3_POINTER_TYPEDEFS(I3SuperNovaTree);
#endif //I3SUPERNOVA_H
