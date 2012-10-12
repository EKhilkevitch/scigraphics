
// =========================================================

#include "test_marker.h"
#include "marker.h"

// =========================================================

CPPUNIT_TEST_SUITE_REGISTRATION( graphics::tests::test_markerLinear );
CPPUNIT_TEST_SUITE_REGISTRATION( graphics::tests::test_markerLogPositive );
CPPUNIT_TEST_SUITE_REGISTRATION( graphics::tests::test_markerLogNegative );

// =========================================================

void graphics::tests::test_markerLinear::marks()
{
  markerLinear Marker;
  Marker.setNumberOfMarks( 3, 7 );
  auto Marks = Marker.marks( interval<number>(0,10) );

  CPPUNIT_ASSERT_EQUAL( (size_t)6,  Marks.size() );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 0,  Marks[0], 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 2,  Marks[1], 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 10, Marks[5], 1e-5 );
  
  Marks = Marker.marks( interval<number>(0,10000) );
  CPPUNIT_ASSERT_EQUAL( (size_t)6,     Marks.size() );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 0,     Marks[0], 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 2000,  Marks[1], 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 10000, Marks[5], 1e-5 );
  
  Marks = Marker.marks( interval<number>(5,5.4) );
  CPPUNIT_ASSERT_EQUAL( (size_t)5,     Marks.size() );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 5,     Marks[0], 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 5.1,   Marks[1], 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 5.4,   Marks[4], 1e-5 );
  
  Marks = Marker.marks( interval<number>(-1e-10,5) );
  CPPUNIT_ASSERT_EQUAL( (size_t)6,   Marks.size() );
  CPPUNIT_ASSERT_EQUAL( 0.0,   Marks[0] );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 1,   Marks[1], 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 5,   Marks[5], 1e-5 );
  
  Marks = Marker.marks( interval<number>(-2,+2) );
  CPPUNIT_ASSERT_EQUAL( (size_t)5,   Marks.size() );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( -2,   Marks[0], 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( -1,   Marks[1], 1e-5 );
  CPPUNIT_ASSERT_EQUAL(  0.0,   Marks[2] );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( +2,   Marks[4], 1e-5 );
}

// =========================================================

void graphics::tests::test_markerLogPositive::marks()
{
  markerLogarithmPositive Marker;
  
  auto Marks = Marker.marks( interval<number>(0.01,100) );
  CPPUNIT_ASSERT_EQUAL( (size_t)5,  Marks.size() );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.01,  Marks[0], 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.10,  Marks[1], 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.0,   Marks[2], 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 10,    Marks[3], 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 100,   Marks[4], 1e-5 );
  
  Marks = Marker.marks( interval<number>(10,100) );
  CPPUNIT_ASSERT_EQUAL( (size_t)4,  Marks.size() );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 10,   Marks[0], 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 20,   Marks[1], 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 50,   Marks[2], 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 100,  Marks[3], 1e-5 );
  
  Marks = Marker.marks( interval<number>(2e-10,10) );

  CPPUNIT_ASSERT_EQUAL( (size_t)5,  Marks.size() );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 1e-8,   Marks[0], 1e-10 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 1e-6,   Marks[1], 1e-8 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 1e-4,   Marks[2], 1e-6 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 1e-2,   Marks[3], 1e-4 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 1e-0,   Marks[4], 1e-2 );
}

// =========================================================

void graphics::tests::test_markerLogNegative::marks()
{
  markerLogarithmNegative Marker;
  
  auto Marks = Marker.marks( interval<number>(-0.01,-100) );
  CPPUNIT_ASSERT_EQUAL( (size_t)5,  Marks.size() );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( -0.01,  Marks[0], 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( -0.10,  Marks[1], 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( -1.0,   Marks[2], 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( -10,    Marks[3], 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( -100,   Marks[4], 1e-5 );
  
  Marks = Marker.marks( interval<number>(-10,-100) );
  CPPUNIT_ASSERT_EQUAL( (size_t)4,  Marks.size() );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( -10,   Marks[0], 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( -20,   Marks[1], 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( -50,   Marks[2], 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( -100,  Marks[3], 1e-5 );
}

// =========================================================
 


