/**
    copyright  (C) 2004
    the icecube collaboration
    $Id: I3MCPMTResponseTest.cxx 28120 2007-01-26 16:32:24Z olivas $

    @version $Revision: 1.8 $
    @date $Date: 2007-01-26 11:32:24 -0500 (Fri, 26 Jan 2007) $
    @author Troy D. Straszheim

    @todo

*/
#include <I3Test.h>
#include <cmath>
#include "dataclasses/physics/I3MCPMTResponse.h"
#include "dataclasses/physics/I3Particle.h"

// this is a typical minimal testsuite

// This string identifies the test suite in the list of test suites.
TEST_GROUP(I3MCPMTResponseTest);

/**
 * Checks assignment/copy construction
 */
// the string here is used to identify this specific test.
TEST(test1)
{
  I3MCPMTResponse h;
  ENSURE(true);
}
