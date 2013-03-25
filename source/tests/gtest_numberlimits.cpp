
// =========================================================

#include <algorithm>
#include <cstring>
#include <cmath>

#include <gtest/gtest.h>

#include "numlimits.h"

namespace scigraphics {};
using namespace scigraphics;

// =========================================================

class test_numberLimits : public testing::Test
{
};

// =========================================================

TEST_F( test_numberLimits, isValidInterval )
{
  interval<number> I1(1,2), I2( invalidNumber(), invalidNumber() );

  ASSERT_TRUE( numberLimits::isValidInterval(I1) );
  ASSERT_TRUE( ! numberLimits::isValidInterval(I2) );
}

// ---------------------------------------------------------

TEST_F( test_numberLimits, clear )
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

TEST_F( test_numberLimits, updateLimitsPosNums )
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

TEST_F( test_numberLimits, updateLimitsNegNums )
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

TEST_F( test_numberLimits, updateLimitsZero )
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

TEST_F( test_numberLimits, updateLimits )
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

// =========================================================

