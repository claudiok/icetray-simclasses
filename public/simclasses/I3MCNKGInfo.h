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

#include <dataclasses/Utility.h>
#include <dataclasses/I3Position.h>
#include <dataclasses/I3Vector.h>

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

I3_POINTER_TYPEDEFS (I3MCNKGPoint);
I3_POINTER_TYPEDEFS (I3MCNKGInfoList);

#endif
