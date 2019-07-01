
// =========================================================

#include <gtest/gtest.h>

#include "marker.h"

using namespace scigraphics;

// =========================================================

TEST( test_markerLinear, marks )
{
  markerLinear Marker;
  Marker.setNumberOfMarks( 3, 7 );
  std::vector<number> Marks1 = Marker.marks( interval<number>(0,10) );

  ASSERT_EQ( 6,  Marks1.size() );
  ASSERT_NEAR( 0,  Marks1[0], 1e-5 );
  ASSERT_NEAR( 2,  Marks1[1], 1e-5 );
  ASSERT_NEAR( 10, Marks1[5], 1e-5 );
  
  std::vector<number> Marks2 = Marker.marks( interval<number>(0,10000) );
  ASSERT_EQ( 6,     Marks2.size() );
  ASSERT_NEAR( 0,     Marks2[0], 1e-5 );
  ASSERT_NEAR( 2000,  Marks2[1], 1e-5 );
  ASSERT_NEAR( 10000, Marks2[5], 1e-5 );
  
  std::vector<number> Marks3 = Marker.marks( interval<number>(5,5.4) );
  ASSERT_EQ( 5,     Marks3.size() );
  ASSERT_NEAR( 5,     Marks3[0], 1e-5 );
  ASSERT_NEAR( 5.1,   Marks3[1], 1e-5 );
  ASSERT_NEAR( 5.4,   Marks3[4], 1e-5 );
  
  std::vector<number> Marks4 = Marker.marks( interval<number>(-1e-10,5) );
  ASSERT_EQ( 6,   Marks4.size() );
  ASSERT_EQ( 0.0,   Marks4[0] );
  ASSERT_NEAR( 1,   Marks4[1], 1e-5 );
  ASSERT_NEAR( 5,   Marks4[5], 1e-5 );
  
  std::vector<number> Marks5 = Marker.marks( interval<number>(-2,+2) );
  ASSERT_EQ( 5,   Marks5.size() );
  ASSERT_NEAR( -2,   Marks5[0], 1e-5 );
  ASSERT_NEAR( -1,   Marks5[1], 1e-5 );
  ASSERT_EQ(  0.0,   Marks5[2] );
  ASSERT_NEAR( +2,   Marks5[4], 1e-5 );
}

// ---------------------------------------------------------

TEST( test_markerLogPositive, marks_1 )
{
  markerLogarithmPositive Marker;
  
  std::vector<number> Marks1 = Marker.marks( interval<number>(0.01,100) );
  ASSERT_EQ( 5,  Marks1.size() );
  ASSERT_NEAR( 0.01,  Marks1[0], 1e-5 );
  ASSERT_NEAR( 0.10,  Marks1[1], 1e-5 );
  ASSERT_NEAR( 1.0,   Marks1[2], 1e-5 );
  ASSERT_NEAR( 10,    Marks1[3], 1e-5 );
  ASSERT_NEAR( 100,   Marks1[4], 1e-5 );
  
  std::vector<number> Marks2 = Marker.marks( interval<number>(10,100) );
  ASSERT_EQ( 4,  Marks2.size() );
  ASSERT_NEAR( 10,   Marks2[0], 1e-5 );
  ASSERT_NEAR( 20,   Marks2[1], 1e-5 );
  ASSERT_NEAR( 50,   Marks2[2], 1e-5 );
  ASSERT_NEAR( 100,  Marks2[3], 1e-5 );
  
  std::vector<number> Marks3 = Marker.marks( interval<number>(2e-10,10) );
  ASSERT_EQ( 5,  Marks3.size() );
  ASSERT_NEAR( 1e-8,   Marks3[0], 1e-10 );
  ASSERT_NEAR( 1e-6,   Marks3[1], 1e-8 );
  ASSERT_NEAR( 1e-4,   Marks3[2], 1e-6 );
  ASSERT_NEAR( 1e-2,   Marks3[3], 1e-4 );
  ASSERT_NEAR( 1e-0,   Marks3[4], 1e-2 );
}

// ---------------------------------------------------------

TEST( test_markerLogNegative, marks )
{
  markerLogarithmNegative Marker;
  
  std::vector<number> Marks1 = Marker.marks( interval<number>(-0.01,-100) );
  ASSERT_EQ( 5,  Marks1.size() );
  ASSERT_NEAR( -0.01,  Marks1[0], 1e-5 );
  ASSERT_NEAR( -0.10,  Marks1[1], 1e-5 );
  ASSERT_NEAR( -1.0,   Marks1[2], 1e-5 );
  ASSERT_NEAR( -10,    Marks1[3], 1e-5 );
  ASSERT_NEAR( -100,   Marks1[4], 1e-5 );
  
  std::vector<number> Marks2 = Marker.marks( interval<number>(-10,-100) );
  ASSERT_EQ( 4,  Marks2.size() );
  ASSERT_NEAR( -10,   Marks2[0], 1e-5 );
  ASSERT_NEAR( -20,   Marks2[1], 1e-5 );
  ASSERT_NEAR( -50,   Marks2[2], 1e-5 );
  ASSERT_NEAR( -100,  Marks2[3], 1e-5 );
}

// =========================================================

