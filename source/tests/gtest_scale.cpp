
// =========================================================

#include <gtest/gtest.h>

#include <cmath>

#define private public
#define protected public

#include "scale.h"

using namespace scigraphics;

// =========================================================

struct test_scaleLinear : public testing::Test
{
  scigraphics::numberLimits createNumberLimits()
  {
    numberLimits Limits;
    Limits.updateLimits( 1 );
    Limits.updateLimits( 11 );
    return Limits;
  }

};

// =========================================================

struct test_scaleLogPositive : public testing::Test
{
  scigraphics::numberLimits createNumberLimits()
  {
    numberLimits Limits;
    Limits.updateLimits( -1 );
    Limits.updateLimits( -2 );
    Limits.updateLimits( 0.01 );
    Limits.updateLimits( 100 );
    return Limits;
  }

};

// =========================================================

struct test_scaleLogNegative : public testing::Test
{
  scigraphics::numberLimits createNumberLimits()
  {
    numberLimits Limits;
    Limits.updateLimits( +1 );
    Limits.updateLimits( +2 );
    Limits.updateLimits( 0 );
    Limits.updateLimits( -0.01 );
    Limits.updateLimits( -100 );
    return Limits;
  }

};

// =========================================================

struct test_scaleSquare : public testing::Test
{
  scigraphics::numberLimits createNumberLimits()
  {
    numberLimits Limits;
    Limits.updateLimits(  0 );
    Limits.updateLimits( +4 );
    return Limits;
  }

};

// =========================================================

TEST_F( test_scaleLinear, numberToPartOfDistance )
{
  scaleLinear  Scale;
  Scale.setNumberLimits( createNumberLimits() );
  
  ASSERT_NEAR( 0,   Scale.numberToPartOfDistance(1), 1e-5 );
  ASSERT_NEAR( 0.5, Scale.numberToPartOfDistance(6), 1e-5 );
  ASSERT_NEAR( 1,   Scale.numberToPartOfDistance(11), 1e-5 );
  
  ASSERT_NEAR( -1,   Scale.numberToPartOfDistance(-9), 1e-5 );
  ASSERT_NEAR( 2,    Scale.numberToPartOfDistance(21), 1e-5 );
}

// ---------------------------------------------------------

TEST_F( test_scaleLinear, partOfDistanceToNumber )
{
  scaleLinear  Scale;
  Scale.setNumberLimits( createNumberLimits() );
  
  ASSERT_NEAR( 1,    Scale.partOfDistanceToNumber(0), 1e-5 );
  ASSERT_NEAR( 6,    Scale.partOfDistanceToNumber(0.5), 1e-5 );
  ASSERT_NEAR( 11,   Scale.partOfDistanceToNumber(1), 1e-5 );
  
  ASSERT_NEAR( -9,   Scale.partOfDistanceToNumber(-1), 1e-5 );
  ASSERT_NEAR( 21,   Scale.partOfDistanceToNumber(2), 1e-5 );
}

// ---------------------------------------------------------

TEST_F( test_scaleLinear, numberToFraction )
{
  scaleLinear  Scale;
  Scale.setNumberLimits( createNumberLimits() );

  ASSERT_NEAR( 0,   Scale.numberToFraction(1), 1e-5 );
  ASSERT_NEAR( 0.5, Scale.numberToFraction(6), 1e-5 );
  ASSERT_NEAR( 1,   Scale.numberToFraction(11), 1e-5 );
  ASSERT_NEAR( 2,   Scale.numberToFraction(21), 1e-5 );
}

// ---------------------------------------------------------

TEST_F( test_scaleLinear, numberToFractionZoom )
{
  scaleLinear  Scale;
  Scale.setNumberLimits( createNumberLimits() );
  Scale.setZoom(2.0);

  ASSERT_NEAR( 0,   Scale.numberToFraction(1), 1e-5 );
  ASSERT_NEAR( 1,   Scale.numberToFraction(6), 1e-5 );
  ASSERT_NEAR( 2,   Scale.numberToFraction(11), 1e-5 );
  ASSERT_NEAR( 4,   Scale.numberToFraction(21), 1e-5 );
}

// ---------------------------------------------------------

TEST_F( test_scaleLinear, numberToFractionShift )
{
  scaleLinear  Scale;
  Scale.setNumberLimits( createNumberLimits() );
  Scale.setShift( 0.25 );

  ASSERT_NEAR( -0.25,  Scale.numberToFraction(1), 1e-5 );
  ASSERT_NEAR( 0.25,   Scale.numberToFraction(6), 1e-5 );
  ASSERT_NEAR( 0.75,   Scale.numberToFraction(11), 1e-5 );
  ASSERT_NEAR( 1.75,   Scale.numberToFraction(21), 1e-5 );
}

// ---------------------------------------------------------

TEST_F( test_scaleLinear, numberToFractionZoomShift )
{
  scaleLinear  Scale;
  Scale.setNumberLimits( createNumberLimits() );
  Scale.setShift( 0.25 );
  Scale.setZoom( 0.5 );

  ASSERT_NEAR( -0.125,  Scale.numberToFraction(1), 1e-5 );
  ASSERT_NEAR( 0.125,   Scale.numberToFraction(6), 1e-5 );
  ASSERT_NEAR( 0.375,   Scale.numberToFraction(11), 1e-5 );
  ASSERT_NEAR( 0.875,   Scale.numberToFraction(21), 1e-5 );
}

// ---------------------------------------------------------

TEST_F( test_scaleLinear, fractionToNumber )
{
  scaleLinear  Scale;
  Scale.setNumberLimits( createNumberLimits() );
  Scale.setShift( 0.25 );
  Scale.setZoom( 0.5 );

  ASSERT_NEAR( 6,  Scale.fractionToNumber(0.125), 1e-5 );
  ASSERT_NEAR( 21, Scale.fractionToNumber(0.875), 1e-5 );
}

// ---------------------------------------------------------

TEST_F( test_scaleLinear, marks )
{
  scaleLinear  Scale;
  Scale.setNumberLimits( createNumberLimits() );

  std::vector<number> Marks = Scale.marks();
  ASSERT_EQ( 5, Marks.size() );
  ASSERT_NEAR(  2.0, Marks[0], 1e-5 );
  ASSERT_NEAR( 10.0, Marks[4], 1e-5 );
}

// ---------------------------------------------------------

TEST_F( test_scaleLogPositive, numberToFraction )
{
  scaleLogarithmPositive Scale;
  Scale.setNumberLimits( createNumberLimits() );
  
  ASSERT_NEAR( 0,    Scale.numberToFraction(0.01), 1e-5 );
  ASSERT_NEAR( 1,    Scale.numberToFraction(100), 1e-5 );
  ASSERT_NEAR( 0.25, Scale.numberToFraction(0.1), 1e-5 );
  ASSERT_NEAR( 0.50, Scale.numberToFraction(1), 1e-5 );
  ASSERT_NEAR( 0.75, Scale.numberToFraction(10), 1e-5 );
  ASSERT_NEAR( 1.25, Scale.numberToFraction(1000), 1e-5 );
  ASSERT_NEAR( -0.25, Scale.numberToFraction(0.001), 1e-5 );
  ASSERT_NEAR( -10,   Scale.numberToFraction(-1), 1e-5 );
}

// ---------------------------------------------------------

TEST_F( test_scaleLogPositive, fractionToNumber )
{
  scaleLogarithmPositive Scale;
  Scale.setNumberLimits( createNumberLimits() );

  ASSERT_NEAR( 0.1,   Scale.fractionToNumber(0.25),  1e-5 );
  ASSERT_NEAR( 100,   Scale.fractionToNumber(1.0),   1e-5 );
  ASSERT_NEAR( 1000,  Scale.fractionToNumber(1.25),  1e-5 );
  ASSERT_NEAR( 0.001, Scale.fractionToNumber(-0.25), 1e-5 );
}

// ---------------------------------------------------------

TEST_F( test_scaleLogPositive, marks )
{
  scaleLogarithmPositive Scale;
  Scale.setNumberLimits( createNumberLimits() );

  std::vector<number> Marks = Scale.marks();
  ASSERT_EQ( 5, Marks.size() );
  ASSERT_NEAR( 0.01, Marks[0], 1e-5 );
  ASSERT_NEAR( 0.1,  Marks[1], 1e-5 );
  ASSERT_NEAR( 1.0,  Marks[2], 1e-5 );
  ASSERT_NEAR( 10,   Marks[3], 1e-5 );
  ASSERT_NEAR( 100,  Marks[4], 1e-5 );
}

// ---------------------------------------------------------

TEST_F( test_scaleLogNegative, numberToFraction )
{
  scaleLogarithmNegative Scale;
  Scale.setNumberLimits( createNumberLimits() );
  
  ASSERT_NEAR( 1,    Scale.numberToFraction(-0.01), 1e-5 );
  ASSERT_NEAR( 0,    Scale.numberToFraction(-100), 1e-5 );
  ASSERT_NEAR( 0.75, Scale.numberToFraction(-0.1), 1e-5 );
  ASSERT_NEAR( 0.50, Scale.numberToFraction(-1), 1e-5 );
  ASSERT_NEAR( 0.25, Scale.numberToFraction(-10), 1e-5 );
  ASSERT_NEAR( -0.25, Scale.numberToFraction(-1000), 1e-5 );
  ASSERT_NEAR( 1.25, Scale.numberToFraction(-0.001), 1e-5 );
  ASSERT_NEAR( +10,   Scale.numberToFraction(+1), 1e-5 );
}

// ---------------------------------------------------------

TEST_F( test_scaleLogNegative, fractionToNumber )
{
  scaleLogarithmNegative Scale;
  Scale.setNumberLimits( createNumberLimits() );

  ASSERT_NEAR( -0.1,   Scale.fractionToNumber(0.75),  1e-5 );
  ASSERT_NEAR( -100,   Scale.fractionToNumber(0.0),   1e-5 );
  ASSERT_NEAR( -1000,  Scale.fractionToNumber(-0.25),  1e-5 );
  ASSERT_NEAR( -0.001, Scale.fractionToNumber(1.25), 1e-5 );
}

// ---------------------------------------------------------

TEST_F( test_scaleLogNegative, marks )
{
  scaleLogarithmNegative Scale;
  Scale.setNumberLimits( createNumberLimits() );

  std::vector<number> Marks = Scale.marks();

  ASSERT_EQ( 5, Marks.size() );
  ASSERT_NEAR( -0.01, Marks[0], 1e-5 );
  ASSERT_NEAR( -0.1,  Marks[1], 1e-5 );
  ASSERT_NEAR( -1.0,  Marks[2], 1e-5 );
  ASSERT_NEAR( -10,   Marks[3], 1e-5 );
  ASSERT_NEAR( -100,  Marks[4], 1e-5 );
}

// ---------------------------------------------------------

TEST_F( test_scaleSquare, numberToFraction )
{
  scaleSquare Scale;
  Scale.setNumberLimits( createNumberLimits() );
  
  ASSERT_NEAR( 0,               Scale.numberToFraction(0), 1e-5 );
  ASSERT_NEAR( 1,               Scale.numberToFraction(4), 1e-5 );
  ASSERT_NEAR( std::sqrt(0.5),  Scale.numberToFraction(2), 1e-5 );
  ASSERT_NEAR( -std::sqrt(0.5), Scale.numberToFraction(-2), 1e-5 );
  ASSERT_NEAR( 0.5,             Scale.numberToFraction(1), 1e-5 );
  ASSERT_NEAR( 2,               Scale.numberToFraction(16), 1e-5 );
}

// ---------------------------------------------------------

TEST_F( test_scaleSquare, fractionToNumber )
{
  scaleSquare Scale;
  Scale.setNumberLimits( createNumberLimits() );

  ASSERT_NEAR( 0,   Scale.fractionToNumber(0.0),  1e-5 );
  ASSERT_NEAR( 4,   Scale.fractionToNumber(1.0),  1e-5 );
  ASSERT_NEAR( 1,   Scale.fractionToNumber(0.5), 1e-5 );
}

// ---------------------------------------------------------

TEST_F( test_scaleSquare, marks )
{
  scaleSquare Scale;
  Scale.setNumberLimits( createNumberLimits() );

  std::vector<number> Marks = Scale.marks();

  ASSERT_EQ( 9, Marks.size() );
  ASSERT_EQ( 0.0,    Marks[0] );
  ASSERT_NEAR( 0.5, Marks[1], 1e-5 );
  ASSERT_NEAR( 1.0, Marks[2], 1e-5 );
  ASSERT_NEAR( 1.5, Marks[3], 1e-5 );
  ASSERT_NEAR( 2.0, Marks[4], 1e-5 );
}

// =========================================================

