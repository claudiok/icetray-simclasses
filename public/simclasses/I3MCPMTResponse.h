/**
 * copyright  (C) 2004
 * the icecube collaboration
 * $Id: I3MCPMTResponse.h 18969 2006-04-25 22:36:30Z olivas $
 *
 * @file I3MCPMTResponse.h
 * @version $Revision: 1.8 $
 * @date $Date: 2006-04-25 18:36:30 -0400 (Tue, 25 Apr 2006) $
 * @author olivas
 */

#ifndef I3MCPMTRESPONSE_H
#define I3MCPMTRESPONSE_H
#include "icetray/I3FrameObject.h"
#include "icetray/I3Tray.h"
#include "dataclasses/I3Map.h"
#include "icetray/OMKey.h"
#include "icetray/I3Units.h"

/**
 * @brief Implementation class for PMT response, simulated by ROMEO
 *
 * This class contains the PMT-level (pre-DAQ/readout) response to the
 * hits in the event.  It represents the voltage produced at the PMT output.
 *
 */
static const unsigned i3mcpmtresponse_version_ = 9999;

typedef std::vector<std::pair<double,int> > compressed_wf_t;
class I3MCPMTResponse : public I3FrameObject
{
  compressed_wf_t compressed_waveform_;
  std::vector<double> waveform_;
  double binSize_;
  double startTime_;
  double endTime_;
  double tolerance_;
  
  public:

  /**
   * constructor
   */
 I3MCPMTResponse(double startTime, double endTime, 
		 double binSize = 1*I3Units::ns):
  binSize_(binSize),startTime_(startTime),endTime_(endTime),
    tolerance_(1e-6*I3Units::mV)
      {};

  I3MCPMTResponse();		 

  /**
   * destructor
   */
   ~I3MCPMTResponse() {};

  /**
   * PMT output voltage as f(time)
   */
   double GetPMTVoltage(double time) const {
     std::vector<double> wf = Decompress();
     if(time < startTime_ || time > endTime_) return(0);
     unsigned long t = (unsigned long)((time-startTime_)/binSize_);
     if(t>=wf.size()) return(0);
     return(wf[t]);
   }

  /**
   * Get the waveform
   */
  std::vector<double> GetWaveform() const {
    return Decompress();
  }
  
  void SetWaveform(const std::vector<double>& wf){ 
    Compress(wf);
  }

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

  double GetTolerance() {return tolerance_;}
  void SetTolerance(double t) {tolerance_= t;}

  private:

  friend class boost::serialization::access;
  template <class Archive> void load(Archive & ar, unsigned version);
  template <class Archive> void save(Archive & ar, unsigned version)const;

  std::vector<double> Decompress() const;
  void Compress(const std::vector<double>&);

};

/** 
 * pointer type to insulate users from memory managemnt issues
 */
I3_POINTER_TYPEDEFS(I3MCPMTResponse);
BOOST_CLASS_VERSION(I3MCPMTResponse,i3mcpmtresponse_version_);

typedef I3Map<OMKey, I3MCPMTResponse> I3MCPMTResponseMap;
I3_POINTER_TYPEDEFS(I3MCPMTResponseMap);

#endif

