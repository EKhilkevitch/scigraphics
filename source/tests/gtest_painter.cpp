
// =========================================================

#include <gtest/gtest.h>

#define private public
#define protected public
#include "mockpainter.h"
#include "painter.h"

namespace scigraphics {};
using namespace scigraphics;

// =========================================================

TEST( test_painter, setDrawer )
{
  painter Painter;
  ASSERT_TRUE( Painter.getDrawer() == nullptr );
  ASSERT_TRUE( ! Painter.ableToDraw() );

  auto Drawer = new mockDrawer();
  Painter.setDrawer( Drawer );
  ASSERT_TRUE( Drawer == Painter.getDrawer() );
  ASSERT_TRUE( Painter.ableToDraw() );
}

// ---------------------------------------------------------

TEST( test_painter, setIndents )
{
  auto Drawer = new mockDrawer();
  painter Painter;

  Painter.setIndents( indents<wcoord>(10, 20, 30, 40) );
  Painter.setDrawer( Drawer );
  Painter.update();

  ASSERT_EQ( 10,  Painter.plotRectangle().left() );
  ASSERT_EQ( 620, Painter.plotRectangle().right() );
  ASSERT_EQ( 40,  Painter.plotRectangle().down() );
  ASSERT_EQ( 450, Painter.plotRectangle().up() );

  Drawer->setWidth(100);
  Painter.update();
  ASSERT_EQ( 10,  Painter.plotRectangle().left() );
  ASSERT_EQ( 80,  Painter.plotRectangle().right() );
}

// ---------------------------------------------------------

TEST( test_painter, update )
{
  auto Drawer = new mockDrawer();
  painter Painter( Drawer );
  Painter.update();
  
  ASSERT_EQ( 640, Painter.plotRectangle().width() );
  ASSERT_EQ( 480, Painter.plotRectangle().height() );

  Drawer->setWidth( 320 );
  Drawer->setHeight( 240 );
  Painter.update();
  
  ASSERT_EQ( 320, Painter.plotRectangle().width() );
  ASSERT_EQ( 240, Painter.plotRectangle().height() );
}

// ---------------------------------------------------------

TEST( test_painter, fpoint2wpoint )
{
  auto Drawer = new mockDrawer();
  painter Painter( Drawer );
  Painter.setIndents( indents<wcoord>(10, 30, 20, 60) );
  Painter.update();
  
  ASSERT_EQ( 600, Painter.plotRectangle().width() );
  ASSERT_EQ( 400, Painter.plotRectangle().height() );
 
  wpoint Point(0,0);

  Point = Painter.fpoint2wpoint( fpoint(0,0) );
  ASSERT_EQ(  10, Point.x() );
  ASSERT_EQ( 420, Point.y() );
  
  Point = Painter.fpoint2wpoint( fpoint(0.5,0) );
  ASSERT_EQ( 310, Point.x() );
  ASSERT_EQ( 420, Point.y() );
  
  Point = Painter.fpoint2wpoint( fpoint(0,0.5) );
  ASSERT_EQ(  10, Point.x() );
  ASSERT_EQ( 220, Point.y() );
  
  Point = Painter.fpoint2wpoint( fpoint(0.1,0.6) );
  ASSERT_EQ(  70, Point.x() );
  ASSERT_EQ( 180, Point.y() );
}

// ---------------------------------------------------------

TEST( test_painter, canSeparatePoints )
{
#if 0
  auto Drawer = new mockDrawer();
  painter Painter( Drawer );
  Painter.update();

  fpoint Pt1( 0.5, 0.25 );
  fpoint Pt2( 0.5, 0.25 - 1e-4 );
  
  ASSERT_TRUE( ! Painter.canSeparateNormPoints(Pt1,Pt1) );
  ASSERT_TRUE( ! Painter.canSeparateNormPoints(Pt1,Pt2) );
  
  Pt2.moveX(0.1);
  ASSERT_TRUE( Painter.canSeparateNormPoints(Pt1,Pt2) );
#endif
}

// ---------------------------------------------------------

TEST( test_painter, textWidth )
{
  auto Drawer = new mockDrawer();
  painter Painter( Drawer );

  textStyle TextStyle;
  TextStyle.setFontSize(10);

  ASSERT_EQ( Drawer->textWidth("xxx",TextStyle), Painter.textWidth("xxx",TextStyle) );
  ASSERT_EQ( 30, Painter.textWidth("xxx",TextStyle) ); // Only for mock drawer
}

// ---------------------------------------------------------

TEST( test_painter, textHeight )
{
  auto Drawer = new mockDrawer();
  painter Painter( Drawer );

  textStyle TextStyle;
  TextStyle.setFontSize(10);

  ASSERT_EQ( Drawer->textHeight("xxx",TextStyle), Painter.textHeight("xxx",TextStyle) );
  ASSERT_EQ( 10, Painter.textHeight("xxx",TextStyle) ); // Only for mock drawer
}

// =========================================================

