/**
 * copyright  (C) 2004
 * the icecube collaboration
 * $Id: I3MCPMTResponse.h 18969 2006-04-25 22:36:30Z olivas $
 *
 * @file I3MCPMTResponse.h
 * @version $Revision: 1.8 $
 * @date $Date: 2006-04-25 18:36:30 -0400 (Tue, 25 Apr 2006) $
 * @author klein
 * @author deyoung
 * @author ehrlich
 */

#ifndef I3MCPMTRESPONSE_H
#define I3MCPMTRESPONSE_H
#include "icetray/I3FrameObject.h"
#include "icetray/I3Tray.h"
#include "icetray/OMKey.h"
#include "dataclasses/I3Map.h"

/**
 * @brief Implementation class for PMT response, simulated by ROMEO
 *
 * This class contains the PMT-level (pre-DAQ/readout) response to the
 * hits in the event.  It represents the voltage produced at the PMT output.
 *
 */
class I3MCPMTResponse : public I3FrameObject
{
  std::vector<double> waveform_;
  double binSize_;
  double startTime_;
  double endTime_;

  public:

  /**
   * constructor
   */
  I3MCPMTResponse() {};

  /**
   * destructor
   */
   ~I3MCPMTResponse() {};

  /**
   * PMT output voltage as f(time)
   */
   double GetPMTVoltage(double time) const{
       if(time < startTime_ || time > endTime_) return(0);
       unsigned long t = (unsigned long)((time-startTime_)/binSize_);
       if(t>=waveform_.size()) return(0);
       return(waveform_[t]);
     }

  /**
   * Get the waveform
   */
  const std::vector<double>& GetWaveform() const {return waveform_;}

  std::vector<double>& GetWaveform() {return waveform_;}

  /**
   * Get bin size
   */
  const double GetBinSize() const {return binSize_;}

  double GetBinSize() {return binSize_;}

  /**
   * Set bin size
   */
  void SetBinSize(double binsize) {binSize_=binsize;}

  /**
   * Get/Set times
   */
  const double GetStartTime() const {return startTime_;}
  double GetStartTime() {return startTime_;}
  void SetStartTime(double t) {startTime_ = t;}

  const double GetEndTime() const {return endTime_;}
  double GetEndTime() {return endTime_;}
  void SetEndTime(double t) {endTime_= t;}

  private:

  friend class boost::serialization::access;
  template <class Archive> void serialize(Archive & ar, unsigned version);
};

/** 
 * pointer type to insulate users from memory managemnt issues
 */
I3_POINTER_TYPEDEFS(I3MCPMTResponse);

typedef I3Map<OMKey, I3MCPMTResponse> I3MCPMTResponseMap;
I3_POINTER_TYPEDEFS(I3MCPMTResponseMap);

#endif

