/**
 * copyright  (C) 2007
 * the IceCube Collaboration
 *
 * @file I3SuperNova.h
 * @version $Revision: 1.0 $
 * @date $Date: 2007-05-08  $
 * @author juancarlos
 *
 */

#ifndef I3SUPERNOVA_H
#define I3SUPERNOVA_H
#include "dataclasses/physics/I3Particle.h"
#include "dataclasses/I3Map.h"
#include "dataclasses/I3Tree.h"
#include "dataclasses/I3Direction.h"

class I3SuperNova 
{

 private:

  unsigned int NumberOfBins_;
  unsigned int NumberOfNeutrinos_;
  unsigned int NumberOfBeta_;
  unsigned int NumberOfPhotons_;
  unsigned int NumberOfPEs_;
  I3Map<OMKey,unsigned int> DOMCount_;

  // coordinates at exit point
  double DeltaT_;
  double Time_;
  double SNdistance_;
  I3Direction SNdirection_;
  I3Map<double,double> EnergyHistogram_;
  I3Particle::ParticleType NuFlavor_;

  friend class boost::serialization::access;
  template <class Archive> void serialize(Archive& ar, unsigned version);

 public:

  I3SuperNova() {};
  inline void SetNumberOfBins(unsigned int nbin) {NumberOfBins_=nbin;}
  inline unsigned int GetNumberOfBins() {return NumberOfBins_;}

  inline void SetNumberOfNeutrinos(unsigned int nnu) {NumberOfNeutrinos_=nnu;}
  inline unsigned int GetNumberOfNeutrinos() {return NumberOfNeutrinos_;}

  inline void SetNumberOfBetas(unsigned int nbeta) {NumberOfBeta_=nbeta;}
  inline unsigned int GetNumberOfBetas() {return NumberOfBeta_;}

  inline void SetNumberOfPhotons(unsigned int ngamma) {NumberOfPhotons_=ngamma;}
  inline unsigned int GetNumberOfPhotons() {return NumberOfPhotons_;}

  inline void SetNumberOfPEs(unsigned int npe) {NumberOfPEs_=npe;}
  inline unsigned int GetNumberOfPEs() {return NumberOfPEs_;}
  
  inline void SetDeltaT(double dt) {DeltaT_=dt;}
  inline double GetDeltaT() {return DeltaT_;}

  inline void SetTime(double t) {Time_=t;}
  inline double GetTime() {return Time_;}

  inline void SetSNdistance(double d) {SNdistance_=d;}
  inline double GetSNdistance() {return SNdistance_;}

  inline void SetSNdirection(I3Direction d) {SNdirection_=d;}
  inline I3Direction& GetSNdirection() {return SNdirection_;}

  inline void SetEnergyHistogram(I3Map<double,double> eh) {EnergyHistogram_=eh;}
  inline I3Map<double,double>& GetEnergyHistogram() {return EnergyHistogram_;}

  inline void SetNuFlavor(I3Particle::ParticleType nf) {NuFlavor_=nf;}
  inline I3Particle::ParticleType GetNuFlavor() {return NuFlavor_;}
};

I3_POINTER_TYPEDEFS(I3SuperNova);

typedef I3Tree<I3SuperNova> I3SuperNovaTree;
I3_POINTER_TYPEDEFS(I3SuperNovaTree);
#endif //I3SUPERNOVA_H
