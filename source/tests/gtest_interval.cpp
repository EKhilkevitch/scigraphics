
// =========================================================

#include <gtest/gtest.h>

#include "interval.h"

#include <string>
#include <sstream>

using namespace scigraphics;

// =========================================================

TEST( interval, output )
{
  std::ostringstream Stream;
  
  Stream << interval<double>( 2, 1.5 );
  EXPECT_EQ( "[ 1.5 .. 2 ]", Stream.str() );
}

// ---------------------------------------------------------

TEST( interval, input )
{
  {
    std::istringstream Stream( "[ -1.4 .. 3 ]" );
    interval<double> Interval;
    Stream >> Interval;
    EXPECT_TRUE( Stream );
    EXPECT_NEAR( -1.4, Interval.min(), 1e-4 );
    EXPECT_NEAR( +3.0, Interval.max(), 1e-4 );
  }

  {
    std::istringstream Stream( "[-1.4 .. 3][4 .. 5]" );
    interval<double> Interval1, Interval2;
    Stream >> Interval1 >> Interval2;
    EXPECT_TRUE( Stream );
    EXPECT_NEAR( -1.4, Interval1.min(), 1e-4 );
    EXPECT_NEAR( +3.0, Interval1.max(), 1e-4 );
    EXPECT_NEAR( +4.0, Interval2.min(), 1e-4 );
    EXPECT_NEAR( +5.0, Interval2.max(), 1e-4 );
  }


  {
    std::istringstream Stream( "[ -1.4 . 3 )" );
    interval<double> Interval(2,4);
    Stream >> Interval;
    EXPECT_FALSE( Stream );
    EXPECT_NEAR( 2, Interval.min(), 1e-4 );
    EXPECT_NEAR( 4, Interval.max(), 1e-4 );
  }
}

// =========================================================

