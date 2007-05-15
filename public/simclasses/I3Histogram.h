/**
 * copyright  (C) 2007
 * the IceCube Collaboration
 *
 * @file I3Histogram.h
 * @version $Revision: 1.1 $
 * @date $Date: 2007/05/15 21:33:43 $
 * @author juancarlos
 *
 */

#ifndef I3HISTOGRAM_H
#define I3HISTOGRAM_H
#include "icetray/I3TrayHeaders.h"
#include "icetray/I3FrameObject.h"

class I3Histogram : public I3FrameObject 
{
 private:

  int nbin_;
  double minbin_,maxbin_;
  bool init_;
  vector<int> hist_;

  friend class boost::serialization::access;
  template <class Archive> void serialize(Archive& ar, unsigned version);

 public:

  I3Histogram();
  I3Histogram(int nbin, double minbin, double maxbin);

  unsigned GetSize() const { return hist_.size(); }
  double GetBinSize() const;
  void SetSize(unsigned size); 
  double GetMinBin() const {return minbin_;}
  void SetMinBin(double minbin);
  double GetMaxBin() const {return maxbin_;}
  void SetMaxBin(double maxbin);

  double GetBinValue(unsigned bin) const;

  double GetMean() const;

  int& operator[](double value); 

};

I3_POINTER_TYPEDEFS(I3Histogram);

#endif //I3HISTOGRAM_H
