/**
 * Copyright (C) 2010
 * The IceCube collaboration
 * ID: $Id: $
 *
 * @file CorsikaLongStep.cxx
 * @version $Rev: $
 * @date $Date: $
 * @author Tilo Waldenmaier
 */

#include "simclasses/CorsikaLongStep.h"
#include <icetray/serialization.h>

CorsikaLongStep::CorsikaLongStep():
  depth(NAN),
  numGamma(0),
  numEMinus(0),
  numEPlus(0),
  numMuMinus(0),
  numMuPlus(0),
  numHadron(0),
  numCharged(0),
  numNuclei(0),
  numCherenkov(0)
{
  
}

CorsikaLongStep::~CorsikaLongStep()
{
  
}

bool CorsikaLongStep::operator==(const CorsikaLongStep& rhs) const
{
  return 
    (depth == rhs.depth) && 
    (numGamma == rhs.numGamma) &&
    (numEMinus == rhs.numEMinus) &&
    (numEPlus == rhs.numEPlus) &&
    (numMuMinus == rhs.numMuMinus) &&
    (numMuPlus == rhs.numMuPlus) &&
    (numHadron == rhs.numHadron) &&
    (numCharged == rhs.numCharged) &&
    (numNuclei == rhs.numNuclei) &&
    (numCherenkov == rhs.numCherenkov)
    ;
}

template <class Archive>
void CorsikaLongStep::serialize(Archive& ar, unsigned version)
{
  if(version>corsikalongstep_version_)
  {
    log_fatal("Attempting to read version %u from file but running version %u of CorsikaLongStep class.",
	      version,
	      corsikalongstep_version_);
  }
  
  ar & make_nvp("depth",        depth);
  ar & make_nvp("numGamma",     numGamma);
  ar & make_nvp("numEMinus",    numEMinus);
  ar & make_nvp("numEPlus",     numEPlus);
  ar & make_nvp("numMuMinus",   numMuMinus);
  ar & make_nvp("numMuPlus",    numMuPlus);
  ar & make_nvp("numHadron",    numHadron);
  ar & make_nvp("numCharged",   numCharged);
  ar & make_nvp("numNuclei",    numNuclei);
  ar & make_nvp("numCherenkov", numCherenkov);
}

I3_SERIALIZABLE(CorsikaLongStep);
