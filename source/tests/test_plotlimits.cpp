
#include "test_plotlimits.h"
#include "graphics/plotlimits.h"
#include "graphics/axisset.h"
#include "graphics/graphsequence.h"

#include <algorithm>
#include <cstring>
#include <memory>
#include <cmath>
#include <typeinfo>

// =========================================================

CPPUNIT_TEST_SUITE_REGISTRATION( scigraphics::tests::test_plotLimits_limitsXY );
CPPUNIT_TEST_SUITE_REGISTRATION( scigraphics::tests::test_plotLimits );

// =========================================================

void scigraphics::tests::test_plotLimits_limitsXY::isAxisSetXY()
{
  CPPUNIT_ASSERT_EQUAL( false, plotLimits::limitsXY::isAxisSetX(NULL) );
  CPPUNIT_ASSERT_EQUAL( false, plotLimits::limitsXY::isAxisSetY(NULL) );

  axisSet *Axis = new axisSetX(0);
  CPPUNIT_ASSERT_EQUAL( true,  plotLimits::limitsXY::isAxisSetX(Axis) );
  CPPUNIT_ASSERT_EQUAL( false, plotLimits::limitsXY::isAxisSetY(Axis) );

  delete Axis;
  Axis = new axisSetY(0);
  CPPUNIT_ASSERT_EQUAL( false, plotLimits::limitsXY::isAxisSetX(Axis) );
  CPPUNIT_ASSERT_EQUAL( true,  plotLimits::limitsXY::isAxisSetY(Axis) );
  
  delete Axis;
}

// ------------------------------------------------------------

void scigraphics::tests::test_plotLimits_limitsXY::set()
{
  axisSet *AxisX = new axisSetX(0);
  axisSet *AxisY = new axisSetY(0);

  numberLimits NumLimits;
  NumLimits.updateLimits(1);
  NumLimits.updateLimits(10);

  plotLimits::limitsXY Limits;
  Limits.set( AxisX, NumLimits );

  CPPUNIT_ASSERT_DOUBLES_EQUAL( 1,  Limits.get(AxisX).totalLimits().min(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 10, Limits.get(AxisX).totalLimits().max(), 1e-5 );
  CPPUNIT_ASSERT_EQUAL( invalidNumber(), Limits.get(AxisY).totalLimits().min() );
  CPPUNIT_ASSERT_EQUAL( invalidNumber(), Limits.get(AxisY).totalLimits().max() );
  
  NumLimits.updateLimits(11);
  Limits.set( AxisX, NumLimits );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 1,  Limits.get(AxisX).totalLimits().min(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 11, Limits.get(AxisX).totalLimits().max(), 1e-5 );
  CPPUNIT_ASSERT_EQUAL( invalidNumber(), Limits.get(AxisY).totalLimits().min() );
  CPPUNIT_ASSERT_EQUAL( invalidNumber(), Limits.get(AxisY).totalLimits().max() );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 1,  Limits.getX().totalLimits().min(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 11, Limits.getX().totalLimits().max(), 1e-5 );
  
  NumLimits.updateLimits(0);
  Limits.set( AxisY, NumLimits );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 1,  Limits.get(AxisX).totalLimits().min(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 11, Limits.get(AxisX).totalLimits().max(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 0,  Limits.get(AxisY).totalLimits().min(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 11, Limits.get(AxisY).totalLimits().max(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 1,  Limits.getX().totalLimits().min(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 11, Limits.getX().totalLimits().max(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 0,  Limits.getY().totalLimits().min(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 11, Limits.getY().totalLimits().max(), 1e-5 );

  delete AxisX, AxisY;
}

// ------------------------------------------------------------

void scigraphics::tests::test_plotLimits_limitsXY::applyStretch()
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
  
  CPPUNIT_ASSERT_DOUBLES_EQUAL( -1, Limits.get(AxisX).totalLimits().min(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 11, Limits.get(AxisX).totalLimits().max(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( -7, Limits.get(AxisY).totalLimits().min(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( +7, Limits.get(AxisY).totalLimits().max(), 1e-5 );

  delete AxisX, AxisY;
}

// =========================================================

void scigraphics::tests::test_plotLimits::setUp()
{
  AxisX = new axisSetX(0);
  AxisY = new axisSetY(0);
}

void scigraphics::tests::test_plotLimits::tearDown()
{
  delete AxisX;
  delete AxisY;
}

// ------------------------------------------------------------

void scigraphics::tests::test_plotLimits::limitsForGraphics()
{
  graphCollection Graphics;
  Graphics.setDefaultAxisSets( AxisX, AxisY );


  auto Graph = Graphics.create<graphSequenceVector>();
  Graph->append( -1, -1.5 );
  Graph->append( invalidNumber(), 4.0 );
  Graph->append( +2, +0.5 );
  
  CPPUNIT_ASSERT_EQUAL( true, plotLimits::limitsXY::isAxisSetX( Graphics.graphAxisSetX(Graph) ) );
  CPPUNIT_ASSERT_EQUAL( true, plotLimits::limitsXY::isAxisSetY( Graphics.graphAxisSetY(Graph) ) );
  
  Graph = Graphics.create<graphSequenceVector>();
  Graph = Graphics.create<graphSequenceVector>();
  
  Graph = Graphics.create<graphSequenceVector>();
  Graph->append( +5.0, -1.0 );
  Graph->append( -7.5, +2.5 );

  plotLimits PlotLimits;
  PlotLimits.setStretchFactor(1);
  auto Limits = PlotLimits.limitsForGraphics( Graphics );
  
  CPPUNIT_ASSERT_DOUBLES_EQUAL( -7.5, Limits.getX().totalLimits().min(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( +5.0, Limits.getX().totalLimits().max(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( -7.5, Limits.getX().negativeLimits().min(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( -1.0, Limits.getX().negativeLimits().max(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( +2.0, Limits.getX().positiveLimits().min(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( +5.0, Limits.getX().positiveLimits().max(), 1e-5 );
  
  CPPUNIT_ASSERT_DOUBLES_EQUAL( -1.5, Limits.getY().totalLimits().min(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( -1.5, Limits.getY().negativeLimits().min(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( -1.0, Limits.getY().negativeLimits().max(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( +0.5, Limits.getY().positiveLimits().min(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( +2.5, Limits.getY().positiveLimits().max(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( +2.5, Limits.getY().totalLimits().max(), 1e-5 );

  PlotLimits.setInterval( AxisX, interval<number>(-3,plotLimits::AutoScaleMax) );
  PlotLimits.setInterval( AxisY, interval<number>(-4,+4) );
  Limits = PlotLimits.limitsForGraphics( Graphics );
  
  CPPUNIT_ASSERT_DOUBLES_EQUAL( -3.0, Limits.getX().totalLimits().min(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( +5.0, Limits.getX().totalLimits().max(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( -3.0, Limits.getX().negativeLimits().min(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( -1.0, Limits.getX().negativeLimits().max(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( +2.0, Limits.getX().positiveLimits().min(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( +5.0, Limits.getX().positiveLimits().max(), 1e-5 );
  
  CPPUNIT_ASSERT_DOUBLES_EQUAL( -4.0, Limits.getY().totalLimits().min(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( -4.0, Limits.getY().negativeLimits().min(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( -1.0, Limits.getY().negativeLimits().max(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( +0.5, Limits.getY().positiveLimits().min(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( +4.0, Limits.getY().positiveLimits().max(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( +4.0, Limits.getY().totalLimits().max(), 1e-5 ); 
}

// ------------------------------------------------------------

void scigraphics::tests::test_plotLimits::limitsForEmptyGraphics()
{
  graphCollection Graphics;
  Graphics.setDefaultAxisSets( AxisX, AxisY );

  plotLimits PlotLimits;
  PlotLimits.setStretchFactor(1);
  auto Limits = PlotLimits.limitsForGraphics( Graphics );

  CPPUNIT_ASSERT_DOUBLES_EQUAL( -1,   Limits.getX().totalLimits().min(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( -0.1, Limits.getX().negativeLimits().max(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( +0.1, Limits.getX().positiveLimits().min(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( +1,   Limits.getX().totalLimits().max(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( -1,   Limits.getY().totalLimits().min(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( -0.1, Limits.getY().negativeLimits().max(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( +0.1, Limits.getY().positiveLimits().min(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( +1,   Limits.getY().totalLimits().max(), 1e-5 );

  Graphics.create<graphSequenceVector>();
  auto Graph = Graphics.create<graphSequenceVector>();
  Graph->append( invalidNumber(), 4 );
  
  Limits = PlotLimits.limitsForGraphics( Graphics );
  
  CPPUNIT_ASSERT_DOUBLES_EQUAL( -1,   Limits.getX().totalLimits().min(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( -0.1, Limits.getX().negativeLimits().max(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( +0.1, Limits.getX().positiveLimits().min(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( +1,   Limits.getX().totalLimits().max(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( -1,   Limits.getY().totalLimits().min(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( -0.1, Limits.getY().negativeLimits().max(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( +0.1, Limits.getY().positiveLimits().min(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( +1,   Limits.getY().totalLimits().max(), 1e-5 );


  CPPUNIT_ASSERT_EQUAL( plotLimits::AutoScaleMin, PlotLimits.getInterval(AxisX).min() );
  CPPUNIT_ASSERT_EQUAL( plotLimits::AutoScaleMax, PlotLimits.getInterval(AxisX).max() );
  CPPUNIT_ASSERT_EQUAL( plotLimits::AutoScaleMin, PlotLimits.getInterval(AxisY).min() );
  CPPUNIT_ASSERT_EQUAL( plotLimits::AutoScaleMax, PlotLimits.getInterval(AxisY).max() );

  PlotLimits.setInterval( AxisX, interval<number>(2,4) );
  PlotLimits.setInterval( AxisY, interval<number>(-3,plotLimits::AutoScaleMax) );
  
  CPPUNIT_ASSERT_DOUBLES_EQUAL(  2, PlotLimits.getInterval(AxisX).min(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL(  4, PlotLimits.getInterval(AxisX).max(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( -3, PlotLimits.getInterval(AxisY).min(), 1e-5 );
  CPPUNIT_ASSERT_EQUAL( plotLimits::AutoScaleMax, PlotLimits.getInterval(AxisY).max() );
  
  Limits = PlotLimits.limitsForGraphics( Graphics );

  CPPUNIT_ASSERT_DOUBLES_EQUAL( +2,   Limits.getX().totalLimits().min(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( -1.0, Limits.getX().negativeLimits().min(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( -0.1, Limits.getX().negativeLimits().max(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( +2.0, Limits.getX().positiveLimits().min(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( +4.0, Limits.getX().positiveLimits().max(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( +4.0, Limits.getX().totalLimits().max(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( -3,   Limits.getY().totalLimits().min(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( -0.1, Limits.getY().negativeLimits().max(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( +0.1, Limits.getY().positiveLimits().min(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( +1,   Limits.getY().totalLimits().max(), 1e-5 );
}

// ------------------------------------------------------------

void scigraphics::tests::test_plotLimits::limitsForGraphicsPositive()
{
  graphCollection Graphics;
  Graphics.setDefaultAxisSets( AxisX, AxisY );

  auto Graph = Graphics.create<graphSequenceVector>();
  Graph->append( 1, 1.5 );
  Graph->append( 2, 2.0 );
  
  Graph = Graphics.create<graphSequenceVector>();
  Graph->append( 5.0, 0.5 );
  Graph->append( 7.5, 2.5 );

  plotLimits PlotLimits;
  PlotLimits.setStretchFactor(1);
  auto Limits = PlotLimits.limitsForGraphics( Graphics );
  
  CPPUNIT_ASSERT_DOUBLES_EQUAL( +1.0, Limits.getX().totalLimits().min(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( -1.0, Limits.getX().negativeLimits().min(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( -0.1, Limits.getX().negativeLimits().max(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( +1.0, Limits.getX().positiveLimits().min(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( +7.5, Limits.getX().positiveLimits().max(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( +7.5, Limits.getX().totalLimits().max(), 1e-5 );
  
  CPPUNIT_ASSERT_DOUBLES_EQUAL( +0.5, Limits.getY().totalLimits().min(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( -1.0, Limits.getY().negativeLimits().min(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( -0.1, Limits.getY().negativeLimits().max(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( +0.5, Limits.getY().positiveLimits().min(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( +2.5, Limits.getY().positiveLimits().max(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( +2.5, Limits.getY().totalLimits().max(), 1e-5 );

  PlotLimits.setInterval( AxisX, interval<number>(-3,5) );
  PlotLimits.setInterval( AxisY, interval<number>(plotLimits::AutoScaleMin,0.2) );
  Limits = PlotLimits.limitsForGraphics( Graphics );
  
  CPPUNIT_ASSERT_DOUBLES_EQUAL( -3.0, Limits.getX().totalLimits().min(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( +5.0, Limits.getX().totalLimits().max(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( -3.0, Limits.getX().negativeLimits().min(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( -3.0, Limits.getX().negativeLimits().max(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( +1.0, Limits.getX().positiveLimits().min(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( +5.0, Limits.getX().positiveLimits().max(), 1e-5 );
  
  CPPUNIT_ASSERT_DOUBLES_EQUAL( +0.2, Limits.getY().totalLimits().min(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( -1.0, Limits.getY().negativeLimits().min(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( -0.1, Limits.getY().negativeLimits().max(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( +0.2, Limits.getY().positiveLimits().min(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( +0.2, Limits.getY().positiveLimits().max(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( +0.2, Limits.getY().totalLimits().max(), 1e-5 ); 
}

// ------------------------------------------------------------

void scigraphics::tests::test_plotLimits::limitsForGraphicsNegative()
{
  graphCollection Graphics;
  Graphics.setDefaultAxisSets( AxisX, AxisY );

  auto Graph = Graphics.create<graphSequenceVector>();

  Graph = Graphics.create<graphSequenceVector>();
  Graph->append( -1, -1.5 );
  Graph->append( -2, -0.5 );
  
  Graph = Graphics.create<graphSequenceVector>();
  Graph->append( -5.0, -1.0 );
  Graph->append( -7.5, -2.5 );

  plotLimits PlotLimits;
  PlotLimits.setStretchFactor(1);
  auto Limits = PlotLimits.limitsForGraphics( Graphics );
  
  CPPUNIT_ASSERT_DOUBLES_EQUAL( -7.5, Limits.getX().totalLimits().min(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( -7.5, Limits.getX().negativeLimits().min(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( -1.0, Limits.getX().negativeLimits().max(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( +0.1, Limits.getX().positiveLimits().min(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( +1.0, Limits.getX().positiveLimits().max(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( -1.0, Limits.getX().totalLimits().max(), 1e-5 );
  
  CPPUNIT_ASSERT_DOUBLES_EQUAL( -2.5, Limits.getY().totalLimits().min(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( -2.5, Limits.getY().negativeLimits().min(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( -0.5, Limits.getY().negativeLimits().max(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( +0.1, Limits.getY().positiveLimits().min(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( +1.0, Limits.getY().positiveLimits().max(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( -0.5, Limits.getY().totalLimits().max(), 1e-5 );

  PlotLimits.setInterval( AxisX, interval<number>(-3,5) );
  PlotLimits.setInterval( AxisY, interval<number>(plotLimits::AutoScaleMin,0.2) );
  Limits = PlotLimits.limitsForGraphics( Graphics );
  
  CPPUNIT_ASSERT_DOUBLES_EQUAL( -3.0, Limits.getX().totalLimits().min(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( +5.0, Limits.getX().totalLimits().max(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( -3.0, Limits.getX().negativeLimits().min(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( -1.0, Limits.getX().negativeLimits().max(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( +5.0, Limits.getX().positiveLimits().min(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( +5.0, Limits.getX().positiveLimits().max(), 1e-5 );
  
  CPPUNIT_ASSERT_DOUBLES_EQUAL( -1.5, Limits.getY().totalLimits().min(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( -1.5, Limits.getY().negativeLimits().min(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( -0.5, Limits.getY().negativeLimits().max(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( +0.2, Limits.getY().positiveLimits().min(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( +0.2, Limits.getY().positiveLimits().max(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( +0.2, Limits.getY().totalLimits().max(), 1e-5 ); 
}

// ------------------------------------------------------------

void scigraphics::tests::test_plotLimits::limitsForGraphicsCloseToZero()
{
  graphCollection Graphics;
  Graphics.setDefaultAxisSets( AxisX, AxisY );

  auto Graph = Graphics.create<graphSequenceVector>();
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
  
  CPPUNIT_ASSERT_DOUBLES_EQUAL( -2.0, Limits.getX().totalLimits().min(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( -2.0, Limits.getX().negativeLimits().min(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( -0.5, Limits.getX().negativeLimits().max(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( +0.5, Limits.getX().positiveLimits().min(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( +2.0, Limits.getX().positiveLimits().max(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( +2.0, Limits.getX().totalLimits().max(), 1e-5 );

  Graphics.clear();
  Graph = Graphics.create<graphSequenceVector>();
  Graph->append( -2,     2 );
  Graph->append( -1,     1 );
  Graph->append( -0.5,   0.5 );
  Graph->append( -1e-36, 1e-36 );
  Graph->append( +0.5,   0.5 );
  Graph->append( +2,     2 );
  
  Limits = PlotLimits.limitsForGraphics( Graphics );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( +2.0, Limits.getY().totalLimits().max(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL(  0.0, Limits.getY().totalLimits().min(), 1e-5 );
}

// ------------------------------------------------------------

void scigraphics::tests::test_plotLimits::limitsForGraphicsStretch()
{ 
  graphCollection Graphics;
  Graphics.setDefaultAxisSets( AxisX, AxisY );

  auto Graph = Graphics.create<graphSequenceVector>();
  Graph->append( -2,     1 );
  Graph->append( -1.5,   2 );
  Graph->append( -1,     5 );
  Graph->append( 0,      1 );
  Graph->append( +1,     1 );
  Graph->append( +2,     1 );

  plotLimits PlotLimits;
  PlotLimits.setStretchFactor(1.1);
  auto Limits = PlotLimits.limitsForGraphics( Graphics );
  
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.1, PlotLimits.stretchFactorX(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.1, PlotLimits.stretchFactorY(), 1e-5 );
  
  CPPUNIT_ASSERT_DOUBLES_EQUAL( -2.4,   Limits.getX().totalLimits().min(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( -2.2,   Limits.getX().negativeLimits().min(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( -1/1.1, Limits.getX().negativeLimits().max(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( +1/1.1, Limits.getX().positiveLimits().min(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( +2.2,   Limits.getX().positiveLimits().max(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( +2.4,   Limits.getX().totalLimits().max(), 1e-5 );
  
  CPPUNIT_ASSERT_DOUBLES_EQUAL( +0.6,   Limits.getY().totalLimits().min(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( -1.1,   Limits.getY().negativeLimits().min(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( -1/11.0,Limits.getY().negativeLimits().max(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( +1/1.1, Limits.getY().positiveLimits().min(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( +5.5,   Limits.getY().positiveLimits().max(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( +5.4,   Limits.getY().totalLimits().max(), 1e-5 ); 

  Graphics.clear();
  Graph = Graphics.create<graphSequenceVector>();
  Graph->append( -2,     1 );
  Graph->append( -1,     1 );
  Graph->append(  0,     1 );
  
  Limits = PlotLimits.limitsForGraphics( Graphics );
  
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.9,    Limits.getY().totalLimits().min(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( -1.1,   Limits.getY().negativeLimits().min(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( -1/11.0,Limits.getY().negativeLimits().max(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( +1/1.1, Limits.getY().positiveLimits().min(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( +1.1,   Limits.getY().positiveLimits().max(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( +1.1,   Limits.getY().totalLimits().max(), 1e-5 ); 
  
  Graphics.clear();
  Graph = Graphics.create<graphSequenceVector>();
  Graph->append( 0, 0 );
  
  Limits = PlotLimits.limitsForGraphics( Graphics );
  
  CPPUNIT_ASSERT_DOUBLES_EQUAL( -0.1,   Limits.getY().totalLimits().min(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( -1.1,   Limits.getY().negativeLimits().min(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( -1/11.0,Limits.getY().negativeLimits().max(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( +1/11.0,Limits.getY().positiveLimits().min(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( +1.1,   Limits.getY().positiveLimits().max(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( +0.1,   Limits.getY().totalLimits().max(), 1e-5 ); 
  
  CPPUNIT_ASSERT_DOUBLES_EQUAL( -0.1,   Limits.getX().totalLimits().min(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( -1.1,   Limits.getX().negativeLimits().min(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( -1/11.0,Limits.getX().negativeLimits().max(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( +1/11.0,Limits.getX().positiveLimits().min(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( +1.1,   Limits.getX().positiveLimits().max(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( +0.1,   Limits.getX().totalLimits().max(), 1e-5 ); 
}

// =========================================================


