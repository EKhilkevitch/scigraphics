
/*
 * Copyright 2011,2012 Evgeniy Khilkevitch 
 * 
 * This file is part of scigraphics.
 * 
 * Scigraphics is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * Scigraphics is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with scigraphics.  If not, see <http://www.gnu.org/licenses/>.
 */


#include "test_numberlimits.h"
#include "numlimits.h"

#include <algorithm>
#include <cstring>
#include <memory>
#include <cmath>

// =========================================================

CPPUNIT_TEST_SUITE_REGISTRATION( scigraphics::tests::test_numberLimits );

// =========================================================

void scigraphics::tests::test_numberLimits::isValidInterval()
{
  interval<number> I1(1,2), I2( invalidNumber(), invalidNumber() );

  CPPUNIT_ASSERT( numberLimits::isValidInterval(I1) );
  CPPUNIT_ASSERT( ! numberLimits::isValidInterval(I2) );
}

// ------------------------------------------------------------

void scigraphics::tests::test_numberLimits::clear()
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

void scigraphics::tests::test_numberLimits::updateLimitsPosNums()
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

void scigraphics::tests::test_numberLimits::updateLimitsNegNums()
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

void scigraphics::tests::test_numberLimits::updateLimitsZero()
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

void scigraphics::tests::test_numberLimits::updateLimits()
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

