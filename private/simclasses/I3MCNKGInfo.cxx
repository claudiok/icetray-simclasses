/*******************************************************************/
/* I3MCNKGInfo.cxx                                                 */
/* Implementation of the NKG info                                  */
/* PN Fri Feb 23 10:21:09 EST 2007                                 */
/* $Id:$ */
/*******************************************************************/

#include <dataclasses/BoostHeaders.h>
#include <simclasses/I3MCNKGInfo.h>



/*******************************************************************/

// a constructor, set the values to NAN
I3MCNKGPoint::I3MCNKGPoint ()
  : Position (NAN, NAN, NAN),
    LD1 (NAN),
    LD2 (NAN) {
}

/*******************************************************************/

// a virtual destructor
I3MCNKGPoint::~I3MCNKGPoint () {
  // wer nix weiss und wer nix kann, f"angt zu simulieren an.
}

/*******************************************************************/

// the streamer necessary for boosting it to a file
template <class Archive> void I3MCNKGPoint::serialize (Archive &ar,
						      unsigned version) {

  ar &make_nvp ("Position", Position);
  ar &make_nvp ("LD1", LD1);
  ar &make_nvp ("LD2", LD2);

}

/*******************************************************************/


I3_SERIALIZABLE (I3MCNKGPoint);
I3_SERIALIZABLE (I3MCNKGInfoList);

/*******************************************************************/
/* here follows the interpolation implementation                   */
/*******************************************************************/


// constructor
I3MCNKGInterpolation::I3MCNKGInterpolation (const I3MCNKGInfoList &nkg_values,
					    const I3Particle &primary) {

  fillGrid (nkg_values, primary);

}

/*******************************************************************/

// destructor
I3MCNKGInterpolation::~I3MCNKGInterpolation () {
}
  
/*******************************************************************/

// fill the grid to enable the interpolations. Calculate the first and
// second derivatives as well
void I3MCNKGInterpolation::fillGrid (const I3MCNKGInfoList &nkg_values,
				     const I3Particle &primary) {

  // we store the logarithm (natural, to avoid the non-existing 10^)
  // of the function value as a function of log (r/r_0) and phi.

  // Don't forget to subtract the core position!

  // some ugly hardcoding: In corsika, we have 10 r bins and 8 phi
  // bins.
  static const int N_R = 10;
  static const int N_PHI = 8;

  // r_0 is the smallest value, usually the negative of the first nkg
  // values. The largest value is at the tenth position

  double r_0 = -nkg_values[0].Position.GetX ()
    - primary.GetPos ().GetX ();
  double r_max = -nkg_values[N_R-1].Position.GetX ()
    - primary.GetPos ().GetX ();

  // here again, we need to hardcode by the values of atan2 (y, x)
  // which is +pi maximally and -3/4 pi minimally
  double phi_0 = atan2 (-1., -1.); // -3/4 pi
  double phi_max = atan2 (0., -1.); // pi

  double x; // carthesian coordinates
  double y;
  double r; // polar coordinates
  double phi;
  double l; // logarithm of the density

  f_table_index_t coords; // the coordinates of a position

  for (I3MCNKGInfoList::const_iterator i_nkg = nkg_values.begin ();
       i_nkg != nkg_values.end ();
       i_nkg++) {

    l = log (i_nkg->LD1);
    x = i_nkg->Position.GetX () - primary.GetPos ().GetX ();
    y = i_nkg->Position.GetY () - primary.GetPos ().GetY ();

    r = sqrt (x * x + y * y);
    phi = atan2 (y, x);

    // this makes the equidistant and eases interpolation
    coords[0] = N_R * int ((log (r/r_0) - log (r_0 / r_0))
			   / (log (r_max / r_0) - log (r_0 / r_0)));
    coords[1] = N_PHI * int ((phi - phi_0) / (phi_max - phi_0));

    f_nkg[coords] = l;

  }

  // now we have the function values ordered by rho = log (r/r_0) and
  // phi. Let's start calculating the derivatives so that we can
  // interpolate
  // the first derivative
  
  for (f_table_t::const_iterator i_nkg = f_nkg.begin ();
       i_nkg != f_nkg.end ();
       i_nkg++) {

  }

}

  
