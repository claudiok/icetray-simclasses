#include <I3Test.h>

// MODIFY: replace with include of the header file for the module you
// are testing, or whatever headers are necessary for this test.

#include <simclasses/SampleClass.h>

TEST_GROUP(simclassesSampleClassTest);

TEST(SomethingAboutSampleClass)
{
  ENSURE(true); // simplest ENSURE.  Use lots of these.
  ENSURE( 1 == 1 );
  ENSURE( 1 != 2 );
  ENSURE(true, "with a comment"); // ENSURE with a comment.  You can use these too.
  FAIL("you need to write tests."); // this will always fail.  
  ENSURE_DISTANCE(1.01, 1.02, 0.011); // check for tolerance
  ENSURE_DISTANCE(1.01, 1.02, 0.011, "with a comment"); // check for tolerance
}

TEST(SomethingElseAboutSampleClass)
{
  // put what you want here
}
