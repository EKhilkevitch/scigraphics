
// =========================================================

#include <algorithm>
#include <cstring>
#include <cmath>
#include <typeinfo>

#include <gtest/gtest.h>

#include "scigraphics/plotlimits.h"
#include "scigraphics/axisset.h"
#include "scigraphics/graphsequence.h"
#include "scigraphics/graphcollection.h"

namespace scigraphics {};
using namespace scigraphics;

// =========================================================

class test_plotLimits_limitsXY : public testing::Test
{
};

// =========================================================

struct test_plotLimits : public testing::Test
{
  axisSet *AxisX, *AxisY;
  void SetUp()
  {
    AxisX = new axisSetX(0);
    AxisY = new axisSetY(0);
  }

  void TearDown()
  {
    delete AxisX;
    delete AxisY;
  }
};

// =========================================================

TEST_F( test_plotLimits_limitsXY, isAxisSetXY )
{
  ASSERT_FALSE( plotLimits::limitsXY::isAxisSetX(NULL) );
  ASSERT_FALSE( plotLimits::limitsXY::isAxisSetY(NULL) );

  axisSet *Axis = new axisSetX(0);
  ASSERT_TRUE(  plotLimits::limitsXY::isAxisSetX(Axis) );
  ASSERT_FALSE( plotLimits::limitsXY::isAxisSetY(Axis) );

  delete Axis;
  Axis = new axisSetY(0);
  ASSERT_FALSE( plotLimits::limitsXY::isAxisSetX(Axis) );
  ASSERT_TRUE(  plotLimits::limitsXY::isAxisSetY(Axis) );
  
  delete Axis;
}

// ---------------------------------------------------------

TEST_F( test_plotLimits_limitsXY, set )
{
  axisSet *AxisX = new axisSetX(0);
  axisSet *AxisY = new axisSetY(0);

  numberLimits NumLimits;
  NumLimits.updateLimits(1);
  NumLimits.updateLimits(10);

  plotLimits::limitsXY Limits;
  Limits.set( AxisX, NumLimits );

  ASSERT_NEAR( 1,  Limits.get(AxisX).totalLimits().min(), 1e-5 );
  ASSERT_NEAR( 10, Limits.get(AxisX).totalLimits().max(), 1e-5 );
  ASSERT_EQ( invalidNumber(), Limits.get(AxisY).totalLimits().min() );
  ASSERT_EQ( invalidNumber(), Limits.get(AxisY).totalLimits().max() );
  
  NumLimits.updateLimits(11);
  Limits.set( AxisX, NumLimits );
  ASSERT_NEAR( 1,  Limits.get(AxisX).totalLimits().min(), 1e-5 );
  ASSERT_NEAR( 11, Limits.get(AxisX).totalLimits().max(), 1e-5 );
  ASSERT_EQ( invalidNumber(), Limits.get(AxisY).totalLimits().min() );
  ASSERT_EQ( invalidNumber(), Limits.get(AxisY).totalLimits().max() );
  ASSERT_NEAR( 1,  Limits.getX().totalLimits().min(), 1e-5 );
  ASSERT_NEAR( 11, Limits.getX().totalLimits().max(), 1e-5 );
  
  NumLimits.updateLimits(0);
  Limits.set( AxisY, NumLimits );
  ASSERT_NEAR( 1,  Limits.get(AxisX).totalLimits().min(), 1e-5 );
  ASSERT_NEAR( 11, Limits.get(AxisX).totalLimits().max(), 1e-5 );
  ASSERT_NEAR( 0,  Limits.get(AxisY).totalLimits().min(), 1e-5 );
  ASSERT_NEAR( 11, Limits.get(AxisY).totalLimits().max(), 1e-5 );
  ASSERT_NEAR( 1,  Limits.getX().totalLimits().min(), 1e-5 );
  ASSERT_NEAR( 11, Limits.getX().totalLimits().max(), 1e-5 );
  ASSERT_NEAR( 0,  Limits.getY().totalLimits().min(), 1e-5 );
  ASSERT_NEAR( 11, Limits.getY().totalLimits().max(), 1e-5 );

  delete AxisX, AxisY;
}

// ---------------------------------------------------------

TEST_F( test_plotLimits_limitsXY, applyStretch )
{
  axisSet *AxisX = new axisSetX(0);
  axisSet *AxisY = new axisSetY(0);

  numberLimits NumLimitsX, NumLimitsY;
  NumLimitsX.updateLimits(0);
  NumLimitsX.updateLimits(10);
  NumLimitsY.updateLimits(-5);
  NumLimitsY.updateLimits(+5);

  plotLimits::limitsXY Limits;
  Limits.set( AxisX, NumLimitsX );
  Limits.set( AxisY, NumLimitsY );
  
  Limits.applyStretch( 1.1, 1.2 );
  
  ASSERT_NEAR( -1, Limits.get(AxisX).totalLimits().min(), 1e-5 );
  ASSERT_NEAR( 11, Limits.get(AxisX).totalLimits().max(), 1e-5 );
  ASSERT_NEAR( -7, Limits.get(AxisY).totalLimits().min(), 1e-5 );
  ASSERT_NEAR( +7, Limits.get(AxisY).totalLimits().max(), 1e-5 );

  delete AxisX, AxisY;
}

// ---------------------------------------------------------

TEST_F( test_plotLimits, limitsForGraphics )
{
  graphCollection Graphics;
  Graphics.setDefaultAxisSets( AxisX, AxisY );


  auto Graph = Graphics.create<sequence::graphVector>();
  Graph->append( -1, -1.5 );
  Graph->append( invalidNumber(), 4.0 );
  Graph->append( +2, +0.5 );
  
  ASSERT_TRUE( plotLimits::limitsXY::isAxisSetX( Graphics.graphAxisSetX(Graph) ) );
  ASSERT_TRUE( plotLimits::limitsXY::isAxisSetY( Graphics.graphAxisSetY(Graph) ) );
  
  Graph = Graphics.create<sequence::graphVector>();
  Graph = Graphics.create<sequence::graphVector>();
  
  Graph = Graphics.create<sequence::graphVector>();
  Graph->append( +5.0, -1.0 );
  Graph->append( -7.5, +2.5 );

  plotLimits PlotLimits;
  PlotLimits.setStretchFactor(1);
  auto Limits = PlotLimits.limitsForGraphics( Graphics );
  
  ASSERT_NEAR( -7.5, Limits.getX().totalLimits().min(), 1e-5 );
  ASSERT_NEAR( +5.0, Limits.getX().totalLimits().max(), 1e-5 );
  ASSERT_NEAR( -7.5, Limits.getX().negativeLimits().min(), 1e-5 );
  ASSERT_NEAR( -1.0, Limits.getX().negativeLimits().max(), 1e-5 );
  ASSERT_NEAR( +2.0, Limits.getX().positiveLimits().min(), 1e-5 );
  ASSERT_NEAR( +5.0, Limits.getX().positiveLimits().max(), 1e-5 );
  
  ASSERT_NEAR( -1.5, Limits.getY().totalLimits().min(), 1e-5 );
  ASSERT_NEAR( -1.5, Limits.getY().negativeLimits().min(), 1e-5 );
  ASSERT_NEAR( -1.0, Limits.getY().negativeLimits().max(), 1e-5 );
  ASSERT_NEAR( +0.5, Limits.getY().positiveLimits().min(), 1e-5 );
  ASSERT_NEAR( +2.5, Limits.getY().positiveLimits().max(), 1e-5 );
  ASSERT_NEAR( +2.5, Limits.getY().totalLimits().max(), 1e-5 );

  PlotLimits.setInterval( AxisX, interval<number>(-3,plotLimits::AutoScaleMax) );
  PlotLimits.setInterval( AxisY, interval<number>(-4,+4) );
  Limits = PlotLimits.limitsForGraphics( Graphics );
  
  ASSERT_NEAR( -3.0, Limits.getX().totalLimits().min(), 1e-5 );
  ASSERT_NEAR( +5.0, Limits.getX().totalLimits().max(), 1e-5 );
  ASSERT_NEAR( -3.0, Limits.getX().negativeLimits().min(), 1e-5 );
  ASSERT_NEAR( -1.0, Limits.getX().negativeLimits().max(), 1e-5 );
  ASSERT_NEAR( +2.0, Limits.getX().positiveLimits().min(), 1e-5 );
  ASSERT_NEAR( +5.0, Limits.getX().positiveLimits().max(), 1e-5 );
  
  ASSERT_NEAR( -4.0, Limits.getY().totalLimits().min(), 1e-5 );
  ASSERT_NEAR( -4.0, Limits.getY().negativeLimits().min(), 1e-5 );
  ASSERT_NEAR( -1.0, Limits.getY().negativeLimits().max(), 1e-5 );
  ASSERT_NEAR( +0.5, Limits.getY().positiveLimits().min(), 1e-5 );
  ASSERT_NEAR( +4.0, Limits.getY().positiveLimits().max(), 1e-5 );
  ASSERT_NEAR( +4.0, Limits.getY().totalLimits().max(), 1e-5 ); 
}

// ---------------------------------------------------------

TEST_F( test_plotLimits, limitsForEmptyGraphics )
{
  graphCollection Graphics;
  Graphics.setDefaultAxisSets( AxisX, AxisY );

  plotLimits PlotLimits;
  PlotLimits.setStretchFactor(1);
  auto Limits = PlotLimits.limitsForGraphics( Graphics );

  ASSERT_NEAR( -1,   Limits.getX().totalLimits().min(), 1e-5 );
  ASSERT_NEAR( -0.1, Limits.getX().negativeLimits().max(), 1e-5 );
  ASSERT_NEAR( +0.1, Limits.getX().positiveLimits().min(), 1e-5 );
  ASSERT_NEAR( +1,   Limits.getX().totalLimits().max(), 1e-5 );
  ASSERT_NEAR( -1,   Limits.getY().totalLimits().min(), 1e-5 );
  ASSERT_NEAR( -0.1, Limits.getY().negativeLimits().max(), 1e-5 );
  ASSERT_NEAR( +0.1, Limits.getY().positiveLimits().min(), 1e-5 );
  ASSERT_NEAR( +1,   Limits.getY().totalLimits().max(), 1e-5 );

  Graphics.create<sequence::graphVector>();
  auto Graph = Graphics.create<sequence::graphVector>();
  Graph->append( invalidNumber(), 4 );
  
  Limits = PlotLimits.limitsForGraphics( Graphics );
  
  ASSERT_NEAR( -1,   Limits.getX().totalLimits().min(), 1e-5 );
  ASSERT_NEAR( -0.1, Limits.getX().negativeLimits().max(), 1e-5 );
  ASSERT_NEAR( +0.1, Limits.getX().positiveLimits().min(), 1e-5 );
  ASSERT_NEAR( +1,   Limits.getX().totalLimits().max(), 1e-5 );
  ASSERT_NEAR( -1,   Limits.getY().totalLimits().min(), 1e-5 );
  ASSERT_NEAR( -0.1, Limits.getY().negativeLimits().max(), 1e-5 );
  ASSERT_NEAR( +0.1, Limits.getY().positiveLimits().min(), 1e-5 );
  ASSERT_NEAR( +1,   Limits.getY().totalLimits().max(), 1e-5 );


  ASSERT_EQ( plotLimits::AutoScaleMin, PlotLimits.getInterval(AxisX).min() );
  ASSERT_EQ( plotLimits::AutoScaleMax, PlotLimits.getInterval(AxisX).max() );
  ASSERT_EQ( plotLimits::AutoScaleMin, PlotLimits.getInterval(AxisY).min() );
  ASSERT_EQ( plotLimits::AutoScaleMax, PlotLimits.getInterval(AxisY).max() );

  PlotLimits.setInterval( AxisX, interval<number>(2,4) );
  PlotLimits.setInterval( AxisY, interval<number>(-3,plotLimits::AutoScaleMax) );
  
  ASSERT_NEAR(  2, PlotLimits.getInterval(AxisX).min(), 1e-5 );
  ASSERT_NEAR(  4, PlotLimits.getInterval(AxisX).max(), 1e-5 );
  ASSERT_NEAR( -3, PlotLimits.getInterval(AxisY).min(), 1e-5 );
  ASSERT_EQ( plotLimits::AutoScaleMax, PlotLimits.getInterval(AxisY).max() );
  
  Limits = PlotLimits.limitsForGraphics( Graphics );

  ASSERT_NEAR( +2,   Limits.getX().totalLimits().min(), 1e-5 );
  ASSERT_NEAR( -1.0, Limits.getX().negativeLimits().min(), 1e-5 );
  ASSERT_NEAR( -0.1, Limits.getX().negativeLimits().max(), 1e-5 );
  ASSERT_NEAR( +2.0, Limits.getX().positiveLimits().min(), 1e-5 );
  ASSERT_NEAR( +4.0, Limits.getX().positiveLimits().max(), 1e-5 );
  ASSERT_NEAR( +4.0, Limits.getX().totalLimits().max(), 1e-5 );
  ASSERT_NEAR( -3,   Limits.getY().totalLimits().min(), 1e-5 );
  ASSERT_NEAR( -0.1, Limits.getY().negativeLimits().max(), 1e-5 );
  ASSERT_NEAR( +0.1, Limits.getY().positiveLimits().min(), 1e-5 );
  ASSERT_NEAR( +1,   Limits.getY().totalLimits().max(), 1e-5 );
}

// ---------------------------------------------------------

TEST_F( test_plotLimits, limitsForGraphicsPositive )
{
  graphCollection Graphics;
  Graphics.setDefaultAxisSets( AxisX, AxisY );

  auto Graph = Graphics.create<sequence::graphVector>();
  Graph->append( 1, 1.5 );
  Graph->append( 2, 2.0 );
  
  Graph = Graphics.create<sequence::graphVector>();
  Graph->append( 5.0, 0.5 );
  Graph->append( 7.5, 2.5 );

  plotLimits PlotLimits;
  PlotLimits.setStretchFactor(1);
  auto Limits = PlotLimits.limitsForGraphics( Graphics );
  
  ASSERT_NEAR( +1.0, Limits.getX().totalLimits().min(), 1e-5 );
  ASSERT_NEAR( -1.0, Limits.getX().negativeLimits().min(), 1e-5 );
  ASSERT_NEAR( -0.1, Limits.getX().negativeLimits().max(), 1e-5 );
  ASSERT_NEAR( +1.0, Limits.getX().positiveLimits().min(), 1e-5 );
  ASSERT_NEAR( +7.5, Limits.getX().positiveLimits().max(), 1e-5 );
  ASSERT_NEAR( +7.5, Limits.getX().totalLimits().max(), 1e-5 );
  
  ASSERT_NEAR( +0.5, Limits.getY().totalLimits().min(), 1e-5 );
  ASSERT_NEAR( -1.0, Limits.getY().negativeLimits().min(), 1e-5 );
  ASSERT_NEAR( -0.1, Limits.getY().negativeLimits().max(), 1e-5 );
  ASSERT_NEAR( +0.5, Limits.getY().positiveLimits().min(), 1e-5 );
  ASSERT_NEAR( +2.5, Limits.getY().positiveLimits().max(), 1e-5 );
  ASSERT_NEAR( +2.5, Limits.getY().totalLimits().max(), 1e-5 );

  PlotLimits.setInterval( AxisX, interval<number>(-3,5) );
  PlotLimits.setInterval( AxisY, interval<number>(plotLimits::AutoScaleMin,0.2) );
  Limits = PlotLimits.limitsForGraphics( Graphics );
  
  ASSERT_NEAR( -3.0, Limits.getX().totalLimits().min(), 1e-5 );
  ASSERT_NEAR( +5.0, Limits.getX().totalLimits().max(), 1e-5 );
  ASSERT_NEAR( -3.0, Limits.getX().negativeLimits().min(), 1e-5 );
  ASSERT_NEAR( -3.0, Limits.getX().negativeLimits().max(), 1e-5 );
  ASSERT_NEAR( +1.0, Limits.getX().positiveLimits().min(), 1e-5 );
  ASSERT_NEAR( +5.0, Limits.getX().positiveLimits().max(), 1e-5 );
  
  ASSERT_NEAR( +0.2, Limits.getY().totalLimits().min(), 1e-5 );
  ASSERT_NEAR( -1.0, Limits.getY().negativeLimits().min(), 1e-5 );
  ASSERT_NEAR( -0.1, Limits.getY().negativeLimits().max(), 1e-5 );
  ASSERT_NEAR( +0.2, Limits.getY().positiveLimits().min(), 1e-5 );
  ASSERT_NEAR( +0.2, Limits.getY().positiveLimits().max(), 1e-5 );
  ASSERT_NEAR( +0.2, Limits.getY().totalLimits().max(), 1e-5 ); 
}

// ---------------------------------------------------------

TEST_F( test_plotLimits, limitsForGraphicsNegative )
{
  graphCollection Graphics;
  Graphics.setDefaultAxisSets( AxisX, AxisY );

  auto Graph = Graphics.create<sequence::graphVector>();

  Graph = Graphics.create<sequence::graphVector>();
  Graph->append( -1, -1.5 );
  Graph->append( -2, -0.5 );
  
  Graph = Graphics.create<sequence::graphVector>();
  Graph->append( -5.0, -1.0 );
  Graph->append( -7.5, -2.5 );

  plotLimits PlotLimits;
  PlotLimits.setStretchFactor(1);
  auto Limits = PlotLimits.limitsForGraphics( Graphics );
  
  ASSERT_NEAR( -7.5, Limits.getX().totalLimits().min(), 1e-5 );
  ASSERT_NEAR( -7.5, Limits.getX().negativeLimits().min(), 1e-5 );
  ASSERT_NEAR( -1.0, Limits.getX().negativeLimits().max(), 1e-5 );
  ASSERT_NEAR( +0.1, Limits.getX().positiveLimits().min(), 1e-5 );
  ASSERT_NEAR( +1.0, Limits.getX().positiveLimits().max(), 1e-5 );
  ASSERT_NEAR( -1.0, Limits.getX().totalLimits().max(), 1e-5 );
  
  ASSERT_NEAR( -2.5, Limits.getY().totalLimits().min(), 1e-5 );
  ASSERT_NEAR( -2.5, Limits.getY().negativeLimits().min(), 1e-5 );
  ASSERT_NEAR( -0.5, Limits.getY().negativeLimits().max(), 1e-5 );
  ASSERT_NEAR( +0.1, Limits.getY().positiveLimits().min(), 1e-5 );
  ASSERT_NEAR( +1.0, Limits.getY().positiveLimits().max(), 1e-5 );
  ASSERT_NEAR( -0.5, Limits.getY().totalLimits().max(), 1e-5 );

  PlotLimits.setInterval( AxisX, interval<number>(-3,5) );
  PlotLimits.setInterval( AxisY, interval<number>(plotLimits::AutoScaleMin,0.2) );
  Limits = PlotLimits.limitsForGraphics( Graphics );
  
  ASSERT_NEAR( -3.0, Limits.getX().totalLimits().min(), 1e-5 );
  ASSERT_NEAR( +5.0, Limits.getX().totalLimits().max(), 1e-5 );
  ASSERT_NEAR( -3.0, Limits.getX().negativeLimits().min(), 1e-5 );
  ASSERT_NEAR( -1.0, Limits.getX().negativeLimits().max(), 1e-5 );
  ASSERT_NEAR( +5.0, Limits.getX().positiveLimits().min(), 1e-5 );
  ASSERT_NEAR( +5.0, Limits.getX().positiveLimits().max(), 1e-5 );
  
  ASSERT_NEAR( -1.5, Limits.getY().totalLimits().min(), 1e-5 );
  ASSERT_NEAR( -1.5, Limits.getY().negativeLimits().min(), 1e-5 );
  ASSERT_NEAR( -0.5, Limits.getY().negativeLimits().max(), 1e-5 );
  ASSERT_NEAR( +0.2, Limits.getY().positiveLimits().min(), 1e-5 );
  ASSERT_NEAR( +0.2, Limits.getY().positiveLimits().max(), 1e-5 );
  ASSERT_NEAR( +0.2, Limits.getY().totalLimits().max(), 1e-5 ); 
}

// ---------------------------------------------------------

TEST_F( test_plotLimits, limitsForGraphicsCloseToZero )
{
  graphCollection Graphics;
  Graphics.setDefaultAxisSets( AxisX, AxisY );

  auto Graph = Graphics.create<sequence::graphVector>();
  Graph->append( -2,     1 );
  Graph->append( -1.5,   1 );
  Graph->append( -1,     1 );
  Graph->append( -0.5,   1 );
  Graph->append( -1e-36, 1 );
  Graph->append( +2e-36, 1 );
  Graph->append( +0.5,   1 );
  Graph->append( +1,     1 );
  Graph->append( +2,     1 );
  
  plotLimits PlotLimits;
  PlotLimits.setStretchFactor(1);
  auto Limits = PlotLimits.limitsForGraphics( Graphics );
  
  ASSERT_NEAR( -2.0, Limits.getX().totalLimits().min(), 1e-5 );
  ASSERT_NEAR( -2.0, Limits.getX().negativeLimits().min(), 1e-5 );
  ASSERT_NEAR( -0.5, Limits.getX().negativeLimits().max(), 1e-5 );
  ASSERT_NEAR( +0.5, Limits.getX().positiveLimits().min(), 1e-5 );
  ASSERT_NEAR( +2.0, Limits.getX().positiveLimits().max(), 1e-5 );
  ASSERT_NEAR( +2.0, Limits.getX().totalLimits().max(), 1e-5 );

  Graphics.clear();
  Graph = Graphics.create<sequence::graphVector>();
  Graph->append( -2,     2 );
  Graph->append( -1,     1 );
  Graph->append( -0.5,   0.5 );
  Graph->append( -1e-36, 1e-36 );
  Graph->append( +0.5,   0.5 );
  Graph->append( +2,     2 );
  
  Limits = PlotLimits.limitsForGraphics( Graphics );
  ASSERT_NEAR( +2.0, Limits.getY().totalLimits().max(), 1e-5 );
  ASSERT_NEAR(  0.0, Limits.getY().totalLimits().min(), 1e-5 );
}

// ---------------------------------------------------------

TEST_F( test_plotLimits, limitsForGraphicsStretch )
{ 
  graphCollection Graphics;
  Graphics.setDefaultAxisSets( AxisX, AxisY );

  auto Graph = Graphics.create<sequence::graphVector>();
  Graph->append( -2,     1 );
  Graph->append( -1.5,   2 );
  Graph->append( -1,     5 );
  Graph->append( 0,      1 );
  Graph->append( +1,     1 );
  Graph->append( +2,     1 );

  plotLimits PlotLimits;
  PlotLimits.setStretchFactor(1.1);
  auto Limits = PlotLimits.limitsForGraphics( Graphics );
  
  ASSERT_NEAR( 1.1, PlotLimits.stretchFactorX(), 1e-5 );
  ASSERT_NEAR( 1.1, PlotLimits.stretchFactorY(), 1e-5 );
  
  ASSERT_NEAR( -2.4,   Limits.getX().totalLimits().min(), 1e-5 );
  ASSERT_NEAR( -2.2,   Limits.getX().negativeLimits().min(), 1e-5 );
  ASSERT_NEAR( -1/1.1, Limits.getX().negativeLimits().max(), 1e-5 );
  ASSERT_NEAR( +1/1.1, Limits.getX().positiveLimits().min(), 1e-5 );
  ASSERT_NEAR( +2.2,   Limits.getX().positiveLimits().max(), 1e-5 );
  ASSERT_NEAR( +2.4,   Limits.getX().totalLimits().max(), 1e-5 );
  
  ASSERT_NEAR( +0.6,   Limits.getY().totalLimits().min(), 1e-5 );
  ASSERT_NEAR( -1.1,   Limits.getY().negativeLimits().min(), 1e-5 );
  ASSERT_NEAR( -1/11.0,Limits.getY().negativeLimits().max(), 1e-5 );
  ASSERT_NEAR( +1/1.1, Limits.getY().positiveLimits().min(), 1e-5 );
  ASSERT_NEAR( +5.5,   Limits.getY().positiveLimits().max(), 1e-5 );
  ASSERT_NEAR( +5.4,   Limits.getY().totalLimits().max(), 1e-5 ); 

  Graphics.clear();
  Graph = Graphics.create<sequence::graphVector>();
  Graph->append( -2,     1 );
  Graph->append( -1,     1 );
  Graph->append(  0,     1 );
  
  Limits = PlotLimits.limitsForGraphics( Graphics );
  
  ASSERT_NEAR( 0.9,    Limits.getY().totalLimits().min(), 1e-5 );
  ASSERT_NEAR( -1.1,   Limits.getY().negativeLimits().min(), 1e-5 );
  ASSERT_NEAR( -1/11.0,Limits.getY().negativeLimits().max(), 1e-5 );
  ASSERT_NEAR( +1/1.1, Limits.getY().positiveLimits().min(), 1e-5 );
  ASSERT_NEAR( +1.1,   Limits.getY().positiveLimits().max(), 1e-5 );
  ASSERT_NEAR( +1.1,   Limits.getY().totalLimits().max(), 1e-5 ); 
  
  Graphics.clear();
  Graph = Graphics.create<sequence::graphVector>();
  Graph->append( 0, 0 );
  
  Limits = PlotLimits.limitsForGraphics( Graphics );
  
  ASSERT_NEAR( -0.1,   Limits.getY().totalLimits().min(), 1e-5 );
  ASSERT_NEAR( -1.1,   Limits.getY().negativeLimits().min(), 1e-5 );
  ASSERT_NEAR( -1/11.0,Limits.getY().negativeLimits().max(), 1e-5 );
  ASSERT_NEAR( +1/11.0,Limits.getY().positiveLimits().min(), 1e-5 );
  ASSERT_NEAR( +1.1,   Limits.getY().positiveLimits().max(), 1e-5 );
  ASSERT_NEAR( +0.1,   Limits.getY().totalLimits().max(), 1e-5 ); 
  
  ASSERT_NEAR( -0.1,   Limits.getX().totalLimits().min(), 1e-5 );
  ASSERT_NEAR( -1.1,   Limits.getX().negativeLimits().min(), 1e-5 );
  ASSERT_NEAR( -1/11.0,Limits.getX().negativeLimits().max(), 1e-5 );
  ASSERT_NEAR( +1/11.0,Limits.getX().positiveLimits().min(), 1e-5 );
  ASSERT_NEAR( +1.1,   Limits.getX().positiveLimits().max(), 1e-5 );
  ASSERT_NEAR( +0.1,   Limits.getX().totalLimits().max(), 1e-5 ); 
}

// =========================================================

