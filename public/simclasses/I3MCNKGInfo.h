/**
 *
 * @file I3MCNKGInfo.h
 *
 * @brief Store the data points for the NKG samples provided by CORSIKA
 * @author Peter Niessen Fri Feb 23 10:09:26 EST 2007
 * @date $Date:$
 * @version $Release:$
 *
 * copyright (C) 2007
 * the IceCube collaboration
 *
 * $Id:$
 */

// multiple include protection
#ifndef I3MNKGINFO_H_INCLUDED
#define I3MNKGINFO_H_INCLUDED

#include <vector>
#include <map>

#include <dataclasses/Utility.h>
#include <dataclasses/I3Position.h>
#include <dataclasses/I3Vector.h>
#include <dataclasses/physics/I3Particle.h>

struct I3MCNKGPoint {

  /**
   * where is this point?
   */
  I3Position Position;

  /**
   * see CORSIKA manual for details on this
   */
  double LD1; /// lateral distribution density observation level 1, in m^-2
  double LD2; /// lateral distribution density observation level 2, in m^-2

  /**
   * a constructor;
   * sets contents to NAN
   */
  I3MCNKGPoint ();

  /**
   * a virtual destructor
   * weiss nix? Macht nix!
   */
  virtual ~I3MCNKGPoint ();

  /**
   * the streamer necessary for boosting it to a file
   */
  template <class Archive> void serialize (Archive &ar, unsigned version);

};

/**
 * aggregate many points into a map
 */
typedef I3Vector<I3MCNKGPoint> I3MCNKGInfoList;

/**
 * A class to do the interpolation of values
 */
class I3MCNKGInterpolation {

 public:

  // constructor
  I3MCNKGInterpolation (const I3MCNKGInfoList &nkg_values,
			const I3Particle &primary);

  // destructor
  ~I3MCNKGInterpolation ();
  

 private:

  // need some typedefs
  typedef std::vector<int> f_table_index_t;
  typedef std::map<f_table_index_t, double> f_table_t;

  // the function value of the nkg density as well as the derivatives
  // necessary for interpolation.
  f_table_t f_nkg;
  f_table_t df_dr;
  f_table_t df_dphi;
  f_table_t d2f_dr_dr;
  f_table_t d2f_dr_dphi;
  f_table_t d2f_dphi_dphi;

  // fill the grid to enable the interpolations
  void fillGrid (const I3MCNKGInfoList &nkg_values,
		 const I3Particle &primary);

  
};

I3_POINTER_TYPEDEFS (I3MCNKGPoint);
I3_POINTER_TYPEDEFS (I3MCNKGInfoList);

#endif
