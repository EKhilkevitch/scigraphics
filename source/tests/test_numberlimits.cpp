
#include "test_numberlimits.h"
#include "numlimits.h"

#include <algorithm>
#include <cstring>
#include <memory>
#include <cmath>

// =========================================================

CPPUNIT_TEST_SUITE_REGISTRATION( graphics::tests::test_numberLimits );

// =========================================================

void graphics::tests::test_numberLimits::isValidInterval()
{
  interval<number> I1(1,2), I2( invalidNumber(), invalidNumber() );

  CPPUNIT_ASSERT( numberLimits::isValidInterval(I1) );
  CPPUNIT_ASSERT( ! numberLimits::isValidInterval(I2) );
}

// ------------------------------------------------------------

void graphics::tests::test_numberLimits::clear()
{
  numberLimits Limits;
  CPPUNIT_ASSERT( ! numberLimits::isValidInterval(Limits.totalLimits()) );
  CPPUNIT_ASSERT( ! numberLimits::isValidInterval(Limits.positiveLimits()) );
 
  Limits.updateLimits(1);
  CPPUNIT_ASSERT( numberLimits::isValidInterval(Limits.totalLimits()) );
  CPPUNIT_ASSERT( numberLimits::isValidInterval(Limits.positiveLimits()) );

  Limits.clear();
  CPPUNIT_ASSERT( ! numberLimits::isValidInterval(Limits.totalLimits()) );
  CPPUNIT_ASSERT( ! numberLimits::isValidInterval(Limits.positiveLimits()) );
}

// ------------------------------------------------------------

void graphics::tests::test_numberLimits::updateLimitsPosNums()
{
  numberLimits Limits;
  CPPUNIT_ASSERT_EQUAL( invalidNumber(), Limits.positiveLimits().min() );
  CPPUNIT_ASSERT_EQUAL( invalidNumber(), Limits.positiveLimits().max() );
  CPPUNIT_ASSERT_EQUAL( invalidNumber(), Limits.negativeLimits().min() );
  CPPUNIT_ASSERT_EQUAL( invalidNumber(), Limits.negativeLimits().max() );
  
  Limits.updateLimits(3.5);
  
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 3.5, Limits.totalLimits().min(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 3.5, Limits.totalLimits().max(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 3.5, Limits.positiveLimits().min(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 3.5, Limits.positiveLimits().max(), 1e-5 );
  CPPUNIT_ASSERT_EQUAL( invalidNumber(), Limits.negativeLimits().min() );
  CPPUNIT_ASSERT_EQUAL( invalidNumber(), Limits.negativeLimits().max() );

  Limits.updateLimits(2.5);
  Limits.updateLimits(3.0);
  Limits.updateLimits(4.5);
  
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 2.5, Limits.totalLimits().min(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 4.5, Limits.totalLimits().max(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 2.5, Limits.positiveLimits().min(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 4.5, Limits.positiveLimits().max(), 1e-5 );
  CPPUNIT_ASSERT_EQUAL( invalidNumber(), Limits.negativeLimits().min() );
  CPPUNIT_ASSERT_EQUAL( invalidNumber(), Limits.negativeLimits().max() );
}

// ------------------------------------------------------------

void graphics::tests::test_numberLimits::updateLimitsNegNums()
{
  numberLimits Limits;
  Limits.updateLimits(-3.5);
  
  CPPUNIT_ASSERT_DOUBLES_EQUAL( -3.5, Limits.totalLimits().min(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( -3.5, Limits.totalLimits().max(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( -3.5, Limits.negativeLimits().min(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( -3.5, Limits.negativeLimits().max(), 1e-5 );

  Limits.updateLimits(-2.5);
  Limits.updateLimits(-3.0);
  Limits.updateLimits(-4.5);
  
  CPPUNIT_ASSERT_DOUBLES_EQUAL( -4.5, Limits.totalLimits().min(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( -2.5, Limits.totalLimits().max(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( -4.5, Limits.negativeLimits().min(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( -2.5, Limits.negativeLimits().max(), 1e-5 );
  CPPUNIT_ASSERT_EQUAL( invalidNumber(), Limits.positiveLimits().min() );
  CPPUNIT_ASSERT_EQUAL( invalidNumber(), Limits.positiveLimits().max() );
}

// ------------------------------------------------------------

void graphics::tests::test_numberLimits::updateLimitsZero()
{
  numberLimits Limits;
  
  Limits.updateLimits(0);
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 0, Limits.totalLimits().min(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 0, Limits.totalLimits().max(), 1e-5 );
  CPPUNIT_ASSERT_EQUAL( invalidNumber(), Limits.positiveLimits().min() );
  CPPUNIT_ASSERT_EQUAL( invalidNumber(), Limits.positiveLimits().max() );
  CPPUNIT_ASSERT_EQUAL( invalidNumber(), Limits.negativeLimits().min() );
  CPPUNIT_ASSERT_EQUAL( invalidNumber(), Limits.negativeLimits().max() );
  
  Limits.updateLimits(1);
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 0, Limits.totalLimits().min(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 1, Limits.totalLimits().max(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 1, Limits.positiveLimits().min(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 1, Limits.positiveLimits().max(), 1e-5 );
  CPPUNIT_ASSERT_EQUAL( invalidNumber(), Limits.negativeLimits().min() );
  CPPUNIT_ASSERT_EQUAL( invalidNumber(), Limits.negativeLimits().max() );
  
  Limits.updateLimits(0.5);
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 0, Limits.totalLimits().min(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 1, Limits.totalLimits().max(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.5, Limits.positiveLimits().min(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 1,   Limits.positiveLimits().max(), 1e-5 );
}

// ------------------------------------------------------------

void graphics::tests::test_numberLimits::updateLimits()
{
  numberLimits Limits;
  CPPUNIT_ASSERT( ! numberLimits::isValidInterval(Limits.totalLimits()) );
  CPPUNIT_ASSERT( ! numberLimits::isValidInterval(Limits.positiveLimits()) );
  CPPUNIT_ASSERT( ! numberLimits::isValidInterval(Limits.negativeLimits()) );
  CPPUNIT_ASSERT( ! Limits.isValid() );
  
  Limits.updateLimits(invalidNumber());
  CPPUNIT_ASSERT( ! Limits.isValid() );

  Limits.updateLimits(1);
  CPPUNIT_ASSERT( Limits.isValid() );
  
  Limits.updateLimits(2);
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.0, Limits.totalLimits().min(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 2.0, Limits.totalLimits().max(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.0, Limits.positiveLimits().min(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 2.0, Limits.positiveLimits().max(), 1e-5 );
  CPPUNIT_ASSERT_EQUAL( invalidNumber(), Limits.negativeLimits().min() );
  CPPUNIT_ASSERT_EQUAL( invalidNumber(), Limits.negativeLimits().max() );
  
  Limits.updateLimits(1.5);
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.0, Limits.totalLimits().min(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 2.0, Limits.totalLimits().max(), 1e-5 );
  
  Limits.updateLimits(0);
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0, Limits.totalLimits().min(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 2.0, Limits.totalLimits().max(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.0, Limits.positiveLimits().min(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 2.0, Limits.positiveLimits().max(), 1e-5 );
  CPPUNIT_ASSERT_EQUAL( invalidNumber(), Limits.negativeLimits().min() );
  CPPUNIT_ASSERT_EQUAL( invalidNumber(), Limits.negativeLimits().max() );
  
  Limits.updateLimits(-3);
  CPPUNIT_ASSERT_DOUBLES_EQUAL( -3.0, Limits.totalLimits().min(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL(  2.0, Limits.totalLimits().max(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL(  1.0, Limits.positiveLimits().min(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL(  2.0, Limits.positiveLimits().max(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( -3.0, Limits.negativeLimits().min(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( -3.0, Limits.negativeLimits().max(), 1e-5 );
  
  Limits.updateLimits(invalidNumber());
  CPPUNIT_ASSERT( Limits.isValid() );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( -3.0, Limits.totalLimits().min(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL(  2.0, Limits.totalLimits().max(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL(  1.0, Limits.positiveLimits().min(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL(  2.0, Limits.positiveLimits().max(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( -3.0, Limits.negativeLimits().min(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( -3.0, Limits.negativeLimits().max(), 1e-5 );
}

// =========================================================

