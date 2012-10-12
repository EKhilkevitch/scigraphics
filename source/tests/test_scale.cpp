
// =========================================================

#include "test_scale.h"
#include "scale.h"
#include <cmath>

// =========================================================

CPPUNIT_TEST_SUITE_REGISTRATION( graphics::tests::test_scaleLinear );
CPPUNIT_TEST_SUITE_REGISTRATION( graphics::tests::test_scaleLogPositive );
CPPUNIT_TEST_SUITE_REGISTRATION( graphics::tests::test_scaleLogNegative );
CPPUNIT_TEST_SUITE_REGISTRATION( graphics::tests::test_scaleSquare );

// =========================================================
        
graphics::numberLimits graphics::tests::test_scaleLinear::createNumberLimits()
{
  numberLimits Limits;
  Limits.updateLimits( 1 );
  Limits.updateLimits( 11 );
  return Limits;
}

// ---------------------------------------------------------

void graphics::tests::test_scaleLinear::numberToPartOfDistance()
{
  scaleLinearMorozov  Scale;
  Scale.setNumberLimits( createNumberLimits() );
  
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 0,   Scale.numberToPartOfDistance(1), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.5, Scale.numberToPartOfDistance(6), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 1,   Scale.numberToPartOfDistance(11), 1e-5 );
  
  CPPUNIT_ASSERT_DOUBLES_EQUAL( -1,   Scale.numberToPartOfDistance(-9), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 2,    Scale.numberToPartOfDistance(21), 1e-5 );
}

// ---------------------------------------------------------

void graphics::tests::test_scaleLinear::partOfDistanceToNumber()
{
  scaleLinearMorozov  Scale;
  Scale.setNumberLimits( createNumberLimits() );
  
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 1,    Scale.partOfDistanceToNumber(0), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 6,    Scale.partOfDistanceToNumber(0.5), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 11,   Scale.partOfDistanceToNumber(1), 1e-5 );
  
  CPPUNIT_ASSERT_DOUBLES_EQUAL( -9,   Scale.partOfDistanceToNumber(-1), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 21,   Scale.partOfDistanceToNumber(2), 1e-5 );
}

// ---------------------------------------------------------

void graphics::tests::test_scaleLinear::numberToFraction()
{
  scaleLinearMorozov  Scale;
  Scale.setNumberLimits( createNumberLimits() );

  CPPUNIT_ASSERT_DOUBLES_EQUAL( 0,   Scale.numberToFraction(1), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.5, Scale.numberToFraction(6), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 1,   Scale.numberToFraction(11), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 2,   Scale.numberToFraction(21), 1e-5 );
}

// ---------------------------------------------------------

void graphics::tests::test_scaleLinear::numberToFractionZoom()
{
  scaleLinearMorozov  Scale;
  Scale.setNumberLimits( createNumberLimits() );
  Scale.setZoom(2.0);

  CPPUNIT_ASSERT_DOUBLES_EQUAL( 0,   Scale.numberToFraction(1), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 1,   Scale.numberToFraction(6), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 2,   Scale.numberToFraction(11), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 4,   Scale.numberToFraction(21), 1e-5 );
}

// ---------------------------------------------------------

void graphics::tests::test_scaleLinear::numberToFractionShift()
{
  scaleLinearMorozov  Scale;
  Scale.setNumberLimits( createNumberLimits() );
  Scale.setShift( 0.25 );

  CPPUNIT_ASSERT_DOUBLES_EQUAL( -0.25,  Scale.numberToFraction(1), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.25,   Scale.numberToFraction(6), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.75,   Scale.numberToFraction(11), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.75,   Scale.numberToFraction(21), 1e-5 );
}

// ---------------------------------------------------------

void graphics::tests::test_scaleLinear::numberToFractionZoomShift()
{
  scaleLinearMorozov  Scale;
  Scale.setNumberLimits( createNumberLimits() );
  Scale.setShift( 0.25 );
  Scale.setZoom( 0.5 );

  CPPUNIT_ASSERT_DOUBLES_EQUAL( -0.125,  Scale.numberToFraction(1), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.125,   Scale.numberToFraction(6), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.375,   Scale.numberToFraction(11), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.875,   Scale.numberToFraction(21), 1e-5 );
}

// ---------------------------------------------------------

void graphics::tests::test_scaleLinear::fractionToNumber()
{
  scaleLinearMorozov  Scale;
  Scale.setNumberLimits( createNumberLimits() );
  Scale.setShift( 0.25 );
  Scale.setZoom( 0.5 );

  CPPUNIT_ASSERT_DOUBLES_EQUAL( 6,  Scale.fractionToNumber(0.125), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 21, Scale.fractionToNumber(0.875), 1e-5 );
}

// ---------------------------------------------------------

void graphics::tests::test_scaleLinear::marks()
{
  scaleLinearMorozov  Scale;
  Scale.setNumberLimits( createNumberLimits() );

  auto Marks = Scale.marks();
  CPPUNIT_ASSERT_EQUAL( (size_t)5, Marks.size() );
  CPPUNIT_ASSERT_DOUBLES_EQUAL(  2.0, Marks[0], 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 10.0, Marks[4], 1e-5 );
}

// =========================================================

graphics::numberLimits graphics::tests::test_scaleLogPositive::createNumberLimits()
{
  numberLimits Limits;
  Limits.updateLimits( -1 );
  Limits.updateLimits( -2 );
  Limits.updateLimits( 0.01 );
  Limits.updateLimits( 100 );
  return Limits;
}

// ---------------------------------------------------------

void graphics::tests::test_scaleLogPositive::numberToFraction()
{
  scaleLogarithmPositive Scale;
  Scale.setNumberLimits( createNumberLimits() );
  
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 0,    Scale.numberToFraction(0.01), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 1,    Scale.numberToFraction(100), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.25, Scale.numberToFraction(0.1), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.50, Scale.numberToFraction(1), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.75, Scale.numberToFraction(10), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.25, Scale.numberToFraction(1000), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( -0.25, Scale.numberToFraction(0.001), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( -10,   Scale.numberToFraction(-1), 1e-5 );
}

// ---------------------------------------------------------

void graphics::tests::test_scaleLogPositive::fractionToNumber()
{
  scaleLogarithmPositive Scale;
  Scale.setNumberLimits( createNumberLimits() );

  CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.1,   Scale.fractionToNumber(0.25),  1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 100,   Scale.fractionToNumber(1.0),   1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 1000,  Scale.fractionToNumber(1.25),  1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.001, Scale.fractionToNumber(-0.25), 1e-5 );
}

// ---------------------------------------------------------

void graphics::tests::test_scaleLogPositive::marks()
{
  scaleLogarithmPositive Scale;
  Scale.setNumberLimits( createNumberLimits() );

  auto Marks = Scale.marks();
  CPPUNIT_ASSERT_EQUAL( (size_t)5, Marks.size() );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.01, Marks[0], 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.1,  Marks[1], 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.0,  Marks[2], 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 10,   Marks[3], 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 100,  Marks[4], 1e-5 );
}

// =========================================================

graphics::numberLimits graphics::tests::test_scaleLogNegative::createNumberLimits()
{
  numberLimits Limits;
  Limits.updateLimits( +1 );
  Limits.updateLimits( +2 );
  Limits.updateLimits( 0 );
  Limits.updateLimits( -0.01 );
  Limits.updateLimits( -100 );
  return Limits;
}

// ---------------------------------------------------------

void graphics::tests::test_scaleLogNegative::numberToFraction()
{
  scaleLogarithmNegative Scale;
  Scale.setNumberLimits( createNumberLimits() );
  
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 1,    Scale.numberToFraction(-0.01), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 0,    Scale.numberToFraction(-100), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.75, Scale.numberToFraction(-0.1), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.50, Scale.numberToFraction(-1), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.25, Scale.numberToFraction(-10), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( -0.25, Scale.numberToFraction(-1000), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.25, Scale.numberToFraction(-0.001), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( +10,   Scale.numberToFraction(+1), 1e-5 );
}

// ---------------------------------------------------------

void graphics::tests::test_scaleLogNegative::fractionToNumber()
{
  scaleLogarithmNegative Scale;
  Scale.setNumberLimits( createNumberLimits() );

  CPPUNIT_ASSERT_DOUBLES_EQUAL( -0.1,   Scale.fractionToNumber(0.75),  1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( -100,   Scale.fractionToNumber(0.0),   1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( -1000,  Scale.fractionToNumber(-0.25),  1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( -0.001, Scale.fractionToNumber(1.25), 1e-5 );
}

// ---------------------------------------------------------

void graphics::tests::test_scaleLogNegative::marks()
{
  scaleLogarithmNegative Scale;
  Scale.setNumberLimits( createNumberLimits() );

  auto Marks = Scale.marks();

  CPPUNIT_ASSERT_EQUAL( (size_t)5, Marks.size() );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( -0.01, Marks[0], 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( -0.1,  Marks[1], 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( -1.0,  Marks[2], 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( -10,   Marks[3], 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( -100,  Marks[4], 1e-5 );
}

// =========================================================

graphics::numberLimits graphics::tests::test_scaleSquare::createNumberLimits()
{
  numberLimits Limits;
  Limits.updateLimits(  0 );
  Limits.updateLimits( +4 );
  return Limits;
}

// ---------------------------------------------------------

void graphics::tests::test_scaleSquare::numberToFraction()
{
  scaleSquare Scale;
  Scale.setNumberLimits( createNumberLimits() );
  
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 0,               Scale.numberToFraction(0), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 1,               Scale.numberToFraction(4), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( std::sqrt(0.5),  Scale.numberToFraction(2), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( -std::sqrt(0.5), Scale.numberToFraction(-2), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.5,             Scale.numberToFraction(1), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 2,               Scale.numberToFraction(16), 1e-5 );
}

// ---------------------------------------------------------

void graphics::tests::test_scaleSquare::fractionToNumber()
{
  scaleSquare Scale;
  Scale.setNumberLimits( createNumberLimits() );

  CPPUNIT_ASSERT_DOUBLES_EQUAL( 0,   Scale.fractionToNumber(0.0),  1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 4,   Scale.fractionToNumber(1.0),  1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 1,   Scale.fractionToNumber(0.5), 1e-5 );
}

// ---------------------------------------------------------

void graphics::tests::test_scaleSquare::marks()
{
  scaleSquare Scale;
  Scale.setNumberLimits( createNumberLimits() );

  auto Marks = Scale.marks();

  CPPUNIT_ASSERT_EQUAL( (size_t)9, Marks.size() );
  CPPUNIT_ASSERT_EQUAL( 0.0,    Marks[0] );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.5, Marks[1], 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.0, Marks[2], 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.5, Marks[3], 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 2.0, Marks[4], 1e-5 );
}

// =========================================================

