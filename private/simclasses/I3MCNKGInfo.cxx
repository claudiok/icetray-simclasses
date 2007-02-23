/*******************************************************************/
/* I3MCNKGInfo.cxx                                                 */
/* Implementation of the NKG info                                  */
/* PN Fri Feb 23 10:21:09 EST 2007                                 */
/* $Id:$ */
/*******************************************************************/

#include <dataclasses/BoostHeaders.h>
#include <simclasses/I3MCNKGInfo.h>



// a constructor, set the values to NAN
I3MCNKGPoint::I3MCNKGPoint ()
  : Position (NAN, NAN, NAN),
    LD1 (NAN),
    LD2 (NAN) {
}

// a virtual destructor
I3MCNKGPoint::~I3MCNKGPoint () {
  // wer nix weiss und wer nix kann, f"angt zu simulieren an.
}

// the streamer necessary for boosting it to a file
template <class Archive> void I3MCNKGPoint::serialize (Archive &ar,
						      unsigned version) {

  ar &make_nvp ("Position", Position);
  ar &make_nvp ("LD1", LD1);
  ar &make_nvp ("LD2", LD2);

}


I3_SERIALIZABLE (I3MCNKGPoint);
I3_SERIALIZABLE (I3MCNKGInfoList);
