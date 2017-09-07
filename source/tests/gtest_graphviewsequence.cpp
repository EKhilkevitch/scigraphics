
// =========================================================

#include <gtest/gtest.h>

#include "scigraphics/graphviewsequence.h"
#include "scigraphics/scale.h"
#include "scigraphics/painter.h"
#include "scigraphics/datasequence.h"
#include "scigraphics/numlimits.h"

#include "mockpainter.h"

#include <cmath>

using namespace scigraphics;

// =========================================================

namespace
{
  template <class T> T sqr( T Number ) { return Number * Number; }

  template <class T> double distance( point<T> P1, point<T> P2 )
  {
    return std::sqrt( sqr( P1.x() - P2.x() ) + sqr( P1.y() - P2.y() ) );
  }
}

// =========================================================

TEST( graphViewLine, draw_1 )
{
  sequence::dataUniformVector Data;
  Data.setStepX( 0.1 );
  Data.append( 2 );
  Data.append( 3 );
  //for ( size_t i = 0; i < 20; i++ )
  //  Data.append( i );
 
  mockDrawer *Drawer = new mockDrawer();
  painter Painter( Drawer );
  Painter.update();

  ASSERT_EQ( wpoint(0,480), Painter.fpoint2wpoint(fpoint(0,0)) );
  ASSERT_EQ( wpoint(640,0), Painter.fpoint2wpoint(fpoint(1,1)) );

  scaleLinear ScaleX, ScaleY;
  ScaleX.setNumberLimits( Data.limitsX() );
  ScaleY.setNumberLimits( Data.limitsY( Data.limitsX().totalLimits() ) );
  pairScales PairScales( ScaleX, ScaleY );

  lineStyle Style;
  sequence::graphViewLine ViewLine( Style );

  ViewLine.draw( Painter, PairScales, Data );

  ASSERT_EQ( 1, Drawer->actions().size() );
  ASSERT_EQ( mockDrawer::action::DrawLine, Drawer->actions()[0].Type );
  ASSERT_EQ( 2, Drawer->actions()[0].Points.size() );
  ASSERT_EQ( wpoint(0,480), Drawer->actions()[0].Points[0] );
  ASSERT_EQ( wpoint(640,0), Drawer->actions()[0].Points[1] );
}

// ---------------------------------------------------------

TEST( graphViewLine, draw_2 )
{
  sequence::dataUniformVector Data;
  Data.setStepX( 0.1 );
  for ( size_t i = 0; i < 20; i++ )
    Data.append( i );
 
  mockDrawer *Drawer = new mockDrawer();
  painter Painter( Drawer );
  Painter.update();
 
  scaleLinear ScaleX, ScaleY;
  ScaleX.setNumberLimits( Data.limitsX() );
  ScaleY.setNumberLimits( Data.limitsY( Data.limitsX().totalLimits() ) );
  pairScales PairScales( ScaleX, ScaleY );
  
  ASSERT_EQ( wpoint(0,480), Painter.fpoint2wpoint(fpoint(0,0)) );
  ASSERT_EQ( wpoint(640,0), Painter.fpoint2wpoint(fpoint(1,1)) );
  ASSERT_NEAR( 0, distance( fpoint(0,0), PairScales.fpoint2npoint( npoint(0,0) ) ), 1e-5 );
  ASSERT_NEAR( 0, Data.valueX(0), 1e-5 );
  ASSERT_NEAR( 0, Data.valueY(0), 1e-5 );
  ASSERT_NEAR( 0.1, Data.valueX(1), 1e-5 );
  ASSERT_NEAR( 1.0, Data.valueY(1), 1e-5 );
  
  ASSERT_NEAR( 0.00,   ScaleX.numberToFraction(0.0), 1e-5 );
  ASSERT_NEAR( 1.0/19, ScaleX.numberToFraction(0.1), 1e-5 );
  ASSERT_NEAR( 1.00,   ScaleX.numberToFraction(1.9), 1e-5 );

  ASSERT_NEAR( 0.00,   ScaleY.numberToFraction(0.0), 1e-5 );
  ASSERT_NEAR( 1.0/19, ScaleY.numberToFraction(1),   1e-5 );
  ASSERT_NEAR( 1.00,   ScaleY.numberToFraction(19),  1e-5 );


  lineStyle Style;
  sequence::graphViewLine ViewLine( Style );

  ViewLine.draw( Painter, PairScales, Data );

  ASSERT_EQ( 19, Drawer->actions().size() );
  for ( size_t i = 0; i < Drawer->actions().size(); i++ )
  {
    const mockDrawer::action &Action = Drawer->actions()[i];
  
    ASSERT_EQ( mockDrawer::action::DrawLine, Action.Type ) << " i = " << i;
    ASSERT_EQ( 2, Action.Points.size() );

    wcoord X1 = 640*i/19, Y1 = 480-480*i/19;
    ASSERT_NEAR( X1, Action.Points[0].x(), 1.01 );
    ASSERT_NEAR( Y1, Action.Points[0].y(), 1.01 );
    
    wcoord X2 = 640*(i+1)/19, Y2 = 480-480*(i+1)/19;
    ASSERT_NEAR( X2, Action.Points[1].x(), 1.01 );
    ASSERT_NEAR( Y2, Action.Points[1].y(), 1.01 );
  }
}

// ---------------------------------------------------------

#if 0
TEST( graphViewLine, draw_3 )
{
  sequence::dataUniformVector Data;
  Data.setStepX( 0.1 );
  for ( size_t i = 0; i < 10001; i++ )
    Data.append( i );
 
  mockDrawer *Drawer = new mockDrawer();
  painter Painter( Drawer );
  Painter.update();

  scaleLinear ScaleX, ScaleY;
  ScaleX.setNumberLimits( Data.limitsX() );
  ScaleY.setNumberLimits( Data.limitsY( Data.limitsX().totalLimits() ) );
  pairScales PairScales( ScaleX, ScaleY );
  
  ASSERT_EQ( wpoint(0,480), Painter.fpoint2wpoint(fpoint(0,0)) );
  ASSERT_EQ( wpoint(640,0), Painter.fpoint2wpoint(fpoint(1,1)) );
  ASSERT_NEAR( 0, distance( fpoint(0,0), PairScales.fpoint2npoint( npoint(0,0) ) ), 1e-5 );
  
  ASSERT_NEAR( 0.00, ScaleX.numberToFraction(0.0),  1e-5 );
  ASSERT_NEAR( 1e-4, ScaleX.numberToFraction(0.1),  1e-5 );
  ASSERT_NEAR( 1.00, ScaleX.numberToFraction(1000), 1e-5 );

  ASSERT_NEAR( 0.00, ScaleY.numberToFraction(0.0),   1e-5 );
  ASSERT_NEAR( 1e-4, ScaleY.numberToFraction(1),     1e-5 );
  ASSERT_NEAR( 1.00, ScaleY.numberToFraction(10000), 1e-5 );


  lineStyle Style;
  sequence::graphViewLine ViewLine( Style );

  ViewLine.draw( Painter, PairScales, Data );

  ASSERT_EQ( 10000, Drawer->actions().size() );
  for ( size_t i = 0; i < Drawer->actions().size(); i++ )
  {
    const mockDrawer::action &Action = Drawer->actions()[i];
  
    ASSERT_EQ( mockDrawer::action::DrawLine, Action.Type ) << " i = " << i;
    ASSERT_EQ( 2, Action.Points.size() );

    wcoord X1 = 640*i/19, Y1 = 480-480*i/19;
    ASSERT_NEAR( X1, Action.Points[0].x(), 1.01 );
    ASSERT_NEAR( Y1, Action.Points[0].y(), 1.01 );
    
    wcoord X2 = 640*(i+1)/19, Y2 = 480-480*(i+1)/19;
    ASSERT_NEAR( X2, Action.Points[1].x(), 1.01 );
    ASSERT_NEAR( Y2, Action.Points[1].y(), 1.01 );
  }
}
#endif

// =========================================================

