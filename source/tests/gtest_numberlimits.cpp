
// =========================================================

#include <algorithm>
#include <cstring>
#include <cmath>

#include <gtest/gtest.h>

#include "numlimits.h"
#include "geometry.h"

using namespace scigraphics;

// =========================================================

namespace
{

  // ---------------------------------------------------------
  
  bool comparePointValueByX( fpoint Point, double Value ) 
  { 
    return Point.x() < Value; 
  }
  
  // ---------------------------------------------------------

  number extractPointX( fpoint Point ) 
  { 
    return Point.x(); 
  }
  
  // ---------------------------------------------------------

}

// =========================================================

TEST( test_numberLimits, isValidInterval )
{
  interval<number> I1(1,2), I2( invalidNumber(), invalidNumber() );

  ASSERT_TRUE( numberLimits::isValidInterval(I1) );
  ASSERT_TRUE( ! numberLimits::isValidInterval(I2) );
}

// ---------------------------------------------------------

TEST( test_numberLimits, clear )
{
  numberLimits Limits;
  ASSERT_TRUE( ! numberLimits::isValidInterval(Limits.totalLimits()) );
  ASSERT_TRUE( ! numberLimits::isValidInterval(Limits.positiveLimits()) );
 
  Limits.updateLimits(1);
  ASSERT_TRUE( numberLimits::isValidInterval(Limits.totalLimits()) );
  ASSERT_TRUE( numberLimits::isValidInterval(Limits.positiveLimits()) );

  Limits.clear();
  ASSERT_TRUE( ! numberLimits::isValidInterval(Limits.totalLimits()) );
  ASSERT_TRUE( ! numberLimits::isValidInterval(Limits.positiveLimits()) );
}

// ---------------------------------------------------------

TEST( test_numberLimits, updateLimitsPosNums )
{
  numberLimits Limits;
  ASSERT_EQ( invalidNumber(), Limits.positiveLimits().min() );
  ASSERT_EQ( invalidNumber(), Limits.positiveLimits().max() );
  ASSERT_EQ( invalidNumber(), Limits.negativeLimits().min() );
  ASSERT_EQ( invalidNumber(), Limits.negativeLimits().max() );
  
  Limits.updateLimits(3.5);
  
  ASSERT_NEAR( 3.5, Limits.totalLimits().min(), 1e-5 );
  ASSERT_NEAR( 3.5, Limits.totalLimits().max(), 1e-5 );
  ASSERT_NEAR( 3.5, Limits.positiveLimits().min(), 1e-5 );
  ASSERT_NEAR( 3.5, Limits.positiveLimits().max(), 1e-5 );
  ASSERT_EQ( invalidNumber(), Limits.negativeLimits().min() );
  ASSERT_EQ( invalidNumber(), Limits.negativeLimits().max() );

  Limits.updateLimits(2.5);
  Limits.updateLimits(3.0);
  Limits.updateLimits(4.5);
  
  ASSERT_NEAR( 2.5, Limits.totalLimits().min(), 1e-5 );
  ASSERT_NEAR( 4.5, Limits.totalLimits().max(), 1e-5 );
  ASSERT_NEAR( 2.5, Limits.positiveLimits().min(), 1e-5 );
  ASSERT_NEAR( 4.5, Limits.positiveLimits().max(), 1e-5 );
  ASSERT_EQ( invalidNumber(), Limits.negativeLimits().min() );
  ASSERT_EQ( invalidNumber(), Limits.negativeLimits().max() );
}

// ---------------------------------------------------------

TEST( test_numberLimits, updateLimitsNegNums )
{
  numberLimits Limits;
  Limits.updateLimits(-3.5);
  
  ASSERT_NEAR( -3.5, Limits.totalLimits().min(), 1e-5 );
  ASSERT_NEAR( -3.5, Limits.totalLimits().max(), 1e-5 );
  ASSERT_NEAR( -3.5, Limits.negativeLimits().min(), 1e-5 );
  ASSERT_NEAR( -3.5, Limits.negativeLimits().max(), 1e-5 );

  Limits.updateLimits(-2.5);
  Limits.updateLimits(-3.0);
  Limits.updateLimits(-4.5);
  
  ASSERT_NEAR( -4.5, Limits.totalLimits().min(), 1e-5 );
  ASSERT_NEAR( -2.5, Limits.totalLimits().max(), 1e-5 );
  ASSERT_NEAR( -4.5, Limits.negativeLimits().min(), 1e-5 );
  ASSERT_NEAR( -2.5, Limits.negativeLimits().max(), 1e-5 );
  ASSERT_EQ( invalidNumber(), Limits.positiveLimits().min() );
  ASSERT_EQ( invalidNumber(), Limits.positiveLimits().max() );
}

// ---------------------------------------------------------

TEST( test_numberLimits, updateLimitsZero )
{
  numberLimits Limits;
  
  Limits.updateLimits(0);
  ASSERT_NEAR( 0, Limits.totalLimits().min(), 1e-5 );
  ASSERT_NEAR( 0, Limits.totalLimits().max(), 1e-5 );
  ASSERT_EQ( invalidNumber(), Limits.positiveLimits().min() );
  ASSERT_EQ( invalidNumber(), Limits.positiveLimits().max() );
  ASSERT_EQ( invalidNumber(), Limits.negativeLimits().min() );
  ASSERT_EQ( invalidNumber(), Limits.negativeLimits().max() );
  
  Limits.updateLimits(1);
  ASSERT_NEAR( 0, Limits.totalLimits().min(), 1e-5 );
  ASSERT_NEAR( 1, Limits.totalLimits().max(), 1e-5 );
  ASSERT_NEAR( 1, Limits.positiveLimits().min(), 1e-5 );
  ASSERT_NEAR( 1, Limits.positiveLimits().max(), 1e-5 );
  ASSERT_EQ( invalidNumber(), Limits.negativeLimits().min() );
  ASSERT_EQ( invalidNumber(), Limits.negativeLimits().max() );
  
  Limits.updateLimits(0.5);
  ASSERT_NEAR( 0, Limits.totalLimits().min(), 1e-5 );
  ASSERT_NEAR( 1, Limits.totalLimits().max(), 1e-5 );
  ASSERT_NEAR( 0.5, Limits.positiveLimits().min(), 1e-5 );
  ASSERT_NEAR( 1,   Limits.positiveLimits().max(), 1e-5 );
}

// ---------------------------------------------------------

TEST( test_numberLimits, updateLimits )
{
  numberLimits Limits;
  ASSERT_TRUE( ! numberLimits::isValidInterval(Limits.totalLimits()) );
  ASSERT_TRUE( ! numberLimits::isValidInterval(Limits.positiveLimits()) );
  ASSERT_TRUE( ! numberLimits::isValidInterval(Limits.negativeLimits()) );
  ASSERT_TRUE( ! Limits.isValid() );
  
  Limits.updateLimits(invalidNumber());
  ASSERT_TRUE( ! Limits.isValid() );

  Limits.updateLimits(1);
  ASSERT_TRUE( Limits.isValid() );
  
  Limits.updateLimits(2);
  ASSERT_NEAR( 1.0, Limits.totalLimits().min(), 1e-5 );
  ASSERT_NEAR( 2.0, Limits.totalLimits().max(), 1e-5 );
  ASSERT_NEAR( 1.0, Limits.positiveLimits().min(), 1e-5 );
  ASSERT_NEAR( 2.0, Limits.positiveLimits().max(), 1e-5 );
  ASSERT_EQ( invalidNumber(), Limits.negativeLimits().min() );
  ASSERT_EQ( invalidNumber(), Limits.negativeLimits().max() );
  
  Limits.updateLimits(1.5);
  ASSERT_NEAR( 1.0, Limits.totalLimits().min(), 1e-5 );
  ASSERT_NEAR( 2.0, Limits.totalLimits().max(), 1e-5 );
  
  Limits.updateLimits(0);
  ASSERT_NEAR( 0.0, Limits.totalLimits().min(), 1e-5 );
  ASSERT_NEAR( 2.0, Limits.totalLimits().max(), 1e-5 );
  ASSERT_NEAR( 1.0, Limits.positiveLimits().min(), 1e-5 );
  ASSERT_NEAR( 2.0, Limits.positiveLimits().max(), 1e-5 );
  ASSERT_EQ( invalidNumber(), Limits.negativeLimits().min() );
  ASSERT_EQ( invalidNumber(), Limits.negativeLimits().max() );
  
  Limits.updateLimits(-3);
  ASSERT_NEAR( -3.0, Limits.totalLimits().min(), 1e-5 );
  ASSERT_NEAR(  2.0, Limits.totalLimits().max(), 1e-5 );
  ASSERT_NEAR(  1.0, Limits.positiveLimits().min(), 1e-5 );
  ASSERT_NEAR(  2.0, Limits.positiveLimits().max(), 1e-5 );
  ASSERT_NEAR( -3.0, Limits.negativeLimits().min(), 1e-5 );
  ASSERT_NEAR( -3.0, Limits.negativeLimits().max(), 1e-5 );
  
  Limits.updateLimits(invalidNumber());
  ASSERT_TRUE( Limits.isValid() );
  ASSERT_NEAR( -3.0, Limits.totalLimits().min(), 1e-5 );
  ASSERT_NEAR(  2.0, Limits.totalLimits().max(), 1e-5 );
  ASSERT_NEAR(  1.0, Limits.positiveLimits().min(), 1e-5 );
  ASSERT_NEAR(  2.0, Limits.positiveLimits().max(), 1e-5 );
  ASSERT_NEAR( -3.0, Limits.negativeLimits().min(), 1e-5 );
  ASSERT_NEAR( -3.0, Limits.negativeLimits().max(), 1e-5 );
}

// ---------------------------------------------------------

TEST( test_numberLimits, updateLimits_2 )
{
  std::vector<double> Numbers;
  for ( int i = -10; i <= 20; i++ )
    Numbers.push_back( i );

  numberLimits Limits;
  Limits.updateLimits< std::vector<double>::const_iterator >( Numbers.begin(), Numbers.end() );
  
  EXPECT_NEAR( -10.0, Limits.totalLimits().min(), 1e-5 );
  EXPECT_NEAR(  20.0, Limits.totalLimits().max(), 1e-5 );
  EXPECT_NEAR(   1.0, Limits.positiveLimits().min(), 1e-5 );
  EXPECT_NEAR(  20.0, Limits.positiveLimits().max(), 1e-5 );
  EXPECT_NEAR( -10.0, Limits.negativeLimits().min(), 1e-5 );
  EXPECT_NEAR( - 1.0, Limits.negativeLimits().max(), 1e-5 );
}

// ---------------------------------------------------------

TEST( test_numberLimits, updateLimitsOrdered )
{
  typedef std::vector<fpoint> pvector;
  pvector Points;
  for ( int i = -10; i <= 20; i++ )
    Points.push_back( fpoint( i, 0.5*i ) );
  
  numberLimits Limits;
  Limits.updateLimitsOrdered( Points.begin(), Points.end(), comparePointValueByX, extractPointX );
  
  EXPECT_NEAR( -10.0, Limits.totalLimits().min(), 1e-5 );
  EXPECT_NEAR(  20.0, Limits.totalLimits().max(), 1e-5 );
  EXPECT_NEAR(   1.0, Limits.positiveLimits().min(), 1e-5 );
  EXPECT_NEAR(  20.0, Limits.positiveLimits().max(), 1e-5 );
  EXPECT_NEAR( -10.0, Limits.negativeLimits().min(), 1e-5 );
  EXPECT_NEAR( - 1.0, Limits.negativeLimits().max(), 1e-5 );
}

// ---------------------------------------------------------

TEST( test_numberLimits, boundsOfInterval )
{
  typedef std::vector<double> dvector;
  dvector Numbers;
  for ( size_t i = 0; i < 20; i++ )
    Numbers.push_back( 0.1 * i );

  std::pair< dvector::const_iterator, dvector::const_iterator > Result;

  Result = numberLimits::boundsOfInterval( Numbers.begin(), Numbers.end(), 0.45, 1.3 + 1e-7 );
  EXPECT_EQ(  5, Result.first - Numbers.begin() );
  EXPECT_EQ( 14, Result.second - Numbers.begin() );
  
  Result = numberLimits::boundsOfInterval( Numbers.begin(), Numbers.end(), -0.45, 1.3 + 1e-7 );
  EXPECT_EQ(  0, Result.first - Numbers.begin() );
  EXPECT_EQ( 14, Result.second - Numbers.begin() );
  
  Result = numberLimits::boundsOfInterval( Numbers.begin(), Numbers.end(), 0.45, 21.3 + 1e-7 );
  EXPECT_EQ(  5, Result.first - Numbers.begin() );
  EXPECT_EQ( 20, Result.second - Numbers.begin() );

//  std::cout << *Result.first << " .. " << *Result.second << std::endl;
}

// ---------------------------------------------------------

TEST( test_numberLimits, boundsOfInterval_2 )
{  
  typedef std::vector<fpoint> pvector;
  
  pvector Points;
  for ( size_t i = 0; i < 20; i++ )
    Points.push_back( fpoint( 0.1 * i, i ) );

  std::pair< pvector::const_iterator, pvector::const_iterator > Result = numberLimits::boundsOfInterval( Points.begin(), Points.end(), 0.45, 1.3 + 1e-7, comparePointValueByX );

  EXPECT_EQ(  5, Result.first - Points.begin() );
  EXPECT_EQ( 14, Result.second - Points.begin() );
}

// ---------------------------------------------------------

TEST( test_numberLimits, boundsOfInterval_3 )
{
  typedef std::vector<int> ivector;
  ivector Numbers;
  for ( size_t i = 0; i < 20; i++ )
    Numbers.push_back( i );
  for ( size_t i = 0; i < 4; i++ )
    Numbers.push_back( 4 );
  std::sort( Numbers.begin(), Numbers.end() );
  
  std::pair< ivector::const_iterator, ivector::const_iterator > Result;

  Result = numberLimits::boundsOfInterval( Numbers.begin(), Numbers.end(), 4, 13 );
  EXPECT_EQ(  4, Result.first - Numbers.begin() );
  EXPECT_EQ( 13+4, Result.second - Numbers.begin() );
  ASSERT_TRUE( Result.second != Numbers.end() );
  EXPECT_EQ( 13, *Result.second );
//  std::cout << *Result.first << " .. " << *Result.second << std::endl;
}

// =========================================================

