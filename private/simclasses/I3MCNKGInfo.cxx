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

// some ugly hardcoding: In corsika, we have 10 r bins and 8 phi
// bins.
const int I3MCNKGInterpolation::N_R (10);
const int I3MCNKGInterpolation::N_PHI (8);


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

const double
I3MCNKGInterpolation::Interpolate (const I3Position &tank_position,
				   const I3Particle &primary) const {

  
  double x; // carthesian coordinates
  double y;
  double r; // polar coordinates
  double phi;

  
  x = tank_position.GetX () - primary.GetPos ().GetX ();
  y = tank_position.GetY () - primary.GetPos ().GetY ();

  r = sqrt (x * x + y * y);
  phi = atan2 (y, x);

  // make sure that r is contained
  if (r < R_0)
    r = R_0;

  double lr = log (r / R_0);

  // need to calculate the h_lr and h_phi
  // get the lowest value below the desired coordinate
  f_table_index_t grid_pos (polar2index (r, phi));
 
  double h_lr = lr - H_LR * grid_pos[0];
  double h_phi = phi - H_PHI * grid_pos[1];

  // look up the derivatives. (Do it this way so that the method can
  // be const.)
  f_table_t::const_iterator f = f_nkg.find (grid_pos);
  f_table_t::const_iterator dlr = df_dlr.find (grid_pos);
  f_table_t::const_iterator dphi = df_dphi.find (grid_pos);
  f_table_t::const_iterator d2lrlr = d2f_dlr_dlr.find (grid_pos);
  f_table_t::const_iterator d2phiphi = d2f_dphi_dphi.find (grid_pos);
  f_table_t::const_iterator d2lrphi = d2f_dlr_dphi.find (grid_pos);

  // check if the necessary values are available
  if (f_nkg.end () == f)
    return -1.;
  if (df_dlr.end () == dlr)
    return -1.;
  if (df_dphi.end () == dphi)
    return -1.;
  if (d2f_dlr_dlr.end () == d2lrlr)
    return -1.;
  if (d2f_dphi_dphi.end () == d2phiphi)
    return -1.;
  if (d2f_dlr_dphi.end () == d2lrphi)
    return -1.;
  

  // f_nkg etc. store logarithm, thus, exponential is required
  return exp (f->second
	      + h_lr * dlr->second
	      + h_phi * dphi->second
	      + 0.5 * (h_lr * h_lr * d2lrlr->second
		       + h_phi * h_phi * d2phiphi->second
		       + 2 * h_lr * h_phi * d2lrphi->second)
	      );

}

/*******************************************************************/

// fill the grid to enable the interpolations. Calculate the first and
// second derivatives as well
void I3MCNKGInterpolation::fillGrid (const I3MCNKGInfoList &nkg_values,
				     const I3Particle &primary) {

  // we store the logarithm (natural, to avoid the non-existing 10^)
  // of the function value as a function of log (r/r_0) and phi.

  // Don't forget to subtract the core position!

  // r_0 is the smallest value, usually the negative of the first nkg
  // values. The largest value is at the tenth position

  R_0 = -nkg_values[0].Position.GetX ()
    - primary.GetPos ().GetX ();
  R_MAX = -nkg_values[N_R-1].Position.GetX ()
    - primary.GetPos ().GetX ();

  // need to make r grid equidistant, thus use logarithm of r
  double lr_0 = log (R_0 / R_0);
  double lr_max = log (R_MAX / R_0);
  H_LR = (lr_max - lr_0) / N_R;

  // here again, we need to hardcode by the values of atan2 (y, x)
  // which is +pi maximally and -3/4 pi minimally
  double phi_0 = atan2 (-1., -1.); // -3/4 pi
  double phi_max = atan2 (0., -1.); // pi
  H_PHI = (phi_max - phi_0) / N_PHI;

  double x; // carthesian coordinates
  double y;
  double r; // polar coordinates
  double phi;
  double l; // logarithm of the density

  for (I3MCNKGInfoList::const_iterator i_nkg = nkg_values.begin ();
       i_nkg != nkg_values.end ();
       i_nkg++) {

    l = log (i_nkg->LD1);
    x = i_nkg->Position.GetX () - primary.GetPos ().GetX ();
    y = i_nkg->Position.GetY () - primary.GetPos ().GetY ();

    r = sqrt (x * x + y * y);
    phi = atan2 (y, x);

    f_nkg[polar2index (r, phi)] = l;

  }

  // now we have the function values ordered by rho = log (r/r_0) and
  // phi. Let's start calculating the first derivatives. Take care of
  // the cyclic nature of the phi coordinate and the edges of the r
  // coordinate.
  // if not at the border:
  // df/dr = 0.5 * (f (x+h, phi) - f (x-h, phi)) / h
  // df/dphi = 0.5 * (f (x, phi+h) - f (x, phi-h)) / h
  // if at the upper border: (i.e. no f(.+h) available)
  // df/dr = (f(x, phi) - f(x-h, phi)) / h
  // df/dphi = (f(r, phi) - f(r, phi-h)) / h
  
  for (f_table_t::const_iterator i_nkg = f_nkg.begin ();
       i_nkg != f_nkg.end ();
       i_nkg++) {

    f_table_index_t lr1 (i_nkg->first);
    f_table_index_t lr2 (i_nkg->first);

    f_table_index_t phi1 (i_nkg->first);
    f_table_index_t phi2 (i_nkg->first);

    // are we at the lower r border?
    switch (i_nkg->first[0]) {
    case 0:
      lr1[0]++;
      df_dlr[i_nkg->first] = (f_nkg[lr1] - i_nkg->second) / H_LR;
      break;
    case N_R - 1:
      lr1[0]--;
      df_dlr[i_nkg->first] = (i_nkg->second - f_nkg[lr1]) / H_LR;
      break;
    default:
      lr1[0]--;
      lr2[0]++;
      df_dlr[i_nkg->first] = 0.5 / H_LR *  (f_nkg[lr2] - f_nkg[lr1]);
      break;
    } // switch

    // for the phi interpolation we can make use of the cyclic nature
    phi1[1] = (phi1[1] + N_PHI - 1) % N_PHI;
    phi2[1] = (phi2[1] + 1) % N_PHI;
    df_dphi[i_nkg->first] = 0.5 / H_PHI * (f_nkg[phi2] - f_nkg[phi1]);

  } // loop to fill the first order derivatives

  // now the second order derivatives
  // no values exist for the border values of lr, so set them to 0 in
  // order to ignore them when interpolating.
  for (f_table_t::const_iterator i_nkg = f_nkg.begin ();
       i_nkg != f_nkg.end ();
       i_nkg++) {

    f_table_index_t lr1 (i_nkg->first);
    f_table_index_t lr2 (i_nkg->first);

    f_table_index_t phi1 (i_nkg->first);
    f_table_index_t phi2 (i_nkg->first);

    // are we at the lower r border?
    switch (i_nkg->first[0]) {
    case 0:
    case N_R - 1:
      d2f_dlr_dlr[i_nkg->first] = 0.;
      break;
    default:
      lr1[0]--;
      lr2[0]++;
      d2f_dlr_dlr[i_nkg->first] =
	(f_nkg[lr2] - 2. * i_nkg->second + f_nkg[lr1]) / (H_LR * H_LR);
      break;
    } // switch

    // for the phi interpolation we can make use of the cyclic nature
    phi1[1] = (phi1[1] + N_PHI - 1) % N_PHI;
    phi2[1] = (phi2[1] + 1) % N_PHI;
    d2f_dphi_dphi[i_nkg->first] =
      (f_nkg[phi2] - 2. * i_nkg->second + f_nkg[phi1]) / (H_PHI * H_PHI);

  } // loop to fill the first order derivatives

  // finally, the mixed derivative
  // no values exist for the border values of lr, so set them to 0 in
  // order to ignore them when interpolating.
  for (f_table_t::const_iterator i_nkg = f_nkg.begin ();
       i_nkg != f_nkg.end ();
       i_nkg++) {

    f_table_index_t philr1 (i_nkg->first);
    f_table_index_t philr2 (i_nkg->first);
    f_table_index_t philr3 (i_nkg->first);
    f_table_index_t philr4 (i_nkg->first);


    // are we at the lower r border?
    switch (i_nkg->first[0]) {
    case 0:
    case N_R - 1:
      d2f_dlr_dphi[i_nkg->first] = 0.;
      break;
    default:
      philr1[0]++;
      philr1[1] = ( philr1[1] + 1 ) % N_PHI;
      philr2[0]--;
      philr2[1] = ( philr2[1] + 1 ) % N_PHI;
      philr3[0]++;
      philr3[1] = ( philr3[1] + N_PHI - 1 ) % N_PHI;
      philr4[0]--;
      philr4[1] = ( philr4[1] + N_PHI - 1 ) % N_PHI;
      d2f_dlr_dphi[i_nkg->first] =
	(f_nkg[philr1] - f_nkg[philr2] - f_nkg[philr3] + f_nkg[philr4])
	/ (H_LR * H_PHI);
      break;
    } // switch

  } // loop to fill the first order derivatives



}

  
/*******************************************************************/

const I3MCNKGInterpolation::f_table_index_t
I3MCNKGInterpolation::polar2index (const double r, const double phi) const {

  f_table_index_t coords (2);
  
  // this makes the equidistant and eases interpolation
  coords[0] = N_R * int ((log (r / R_0) - log (R_0 / R_0))
			 / (log (R_MAX / R_0) - log (R_0 / R_0)));
  coords[1] = N_PHI * int ((phi - PHI_0) / (PHI_MAX - PHI_0));


  return coords;

}

