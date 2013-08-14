/**
 * Copyright (C) 2010
 * The IceCube collaboration
 * ID: $Id: $
 *
 * @file I3CorsikaShowerInfo.cxx
 * @version $Rev: $
 * @date $Date: $
 * @author Tilo Waldenmaier
 */


#include "simclasses/I3CorsikaShowerInfo.h"
#include <icetray/serialization.h>


I3CorsikaShowerInfo::I3CorsikaShowerInfo():I3FrameObject()
{
  clear();
}


I3CorsikaShowerInfo::~I3CorsikaShowerInfo()
{
  
}


void I3CorsikaShowerInfo::clear()
{
  crsRunID    = -1;
  crsEventID  = -1;
  crsSampleID = -1;

  firstIntHeight = NAN;
  firstIntDepth  = NAN;
  obsLevelHeight = NAN;
  
  ghMaxNum     = NAN;
  ghStartDepth = NAN;
  ghMaxDepth   = NAN;
  ghRedChiSqr  = NAN;

  resampleRadius = NAN;
  nResample      = 0;
  nResampleNominal = 0;
  weight      = 1.;
  
  longProfile.clear();
}


template <class Archive>
void I3CorsikaShowerInfo::serialize(Archive& ar, unsigned version)
{
  if(version>i3corsikashowerinfo_version_)
  {
    log_fatal("Attempting to read version %u from file but running version %u of I3CorsikaShowerInfo class.",
	      version,
	      i3corsikashowerinfo_version_);
  }
  
  ar & make_nvp("I3FrameObject", base_object<I3FrameObject>(*this));
  ar & make_nvp("crsRunID",       crsRunID);
  ar & make_nvp("crsEventID",     crsEventID);
  ar & make_nvp("crsSampleID",    crsSampleID); 
  ar & make_nvp("firstIntHeight", firstIntHeight);
  ar & make_nvp("firstIntDepth",  firstIntDepth);
  ar & make_nvp("obsLevelHeight", obsLevelHeight);
  ar & make_nvp("ghMaxNum",       ghMaxNum);
  ar & make_nvp("ghStartDepth",   ghStartDepth);
  ar & make_nvp("ghMaxDepth",     ghMaxDepth);
  ar & make_nvp("ghRedChiSqr",    ghRedChiSqr);
  ar & make_nvp("longProfile",    longProfile);

  if (version > 0) {
    ar & make_nvp("resampleRadius", resampleRadius);
    ar & make_nvp("nResample", nResample);
  }
  if (version > 1) {
    ar & make_nvp("weight", weight);
    ar & make_nvp("nResampleNominal", nResampleNominal);
  }
}


I3_SERIALIZABLE(I3CorsikaShowerInfo);
