
// =========================================================

#include "test_painter.h"
#include "painter.h"

// =========================================================

CPPUNIT_TEST_SUITE_REGISTRATION( scigraphics::tests::test_painter );

// =========================================================

void scigraphics::tests::mockDrawer::drawLine( const wpoint &A, const wpoint &B, const lineStyle& Style )
{
  action Act;
  Act.Type = action::DrawLine;
  Act.Points.push_back(A);
  Act.Points.push_back(B);
  Act.Color = Style.getColor();
  Actions.push_back(Act);
}

// ---------------------------------------------------------

void scigraphics::tests::mockDrawer::drawRectangle( const wrectangle& Rectangle, const brushStyle& BrushStyle, const lineStyle &LineStyle )
{
  action Act;
  Act.Type = action::DrawRectangle;
  Act.Points.push_back(Rectangle.leftUp());
  Act.Points.push_back(Rectangle.rightDown());
  Act.Color = BrushStyle.getColor();
  Actions.push_back(Act);
}

// ---------------------------------------------------------

void scigraphics::tests::mockDrawer::drawText( const std::string &Text, const wrectangle& Rectangle, const textStyle &Style, double Angle )
{
}

// =========================================================

void scigraphics::tests::test_painter::setDrawer()
{
  painterMorozov Painter;
  CPPUNIT_ASSERT( Painter.getDrawer() == nullptr );
  CPPUNIT_ASSERT( ! Painter.ableToDraw() );

  auto Drawer = new mockDrawer();
  Painter.setDrawer( Drawer );
  CPPUNIT_ASSERT( Drawer == Painter.getDrawer() );
  CPPUNIT_ASSERT( Painter.ableToDraw() );
}

// ---------------------------------------------------------

void scigraphics::tests::test_painter::setIndents()
{
  auto Drawer = new mockDrawer();
  painterMorozov Painter;

  Painter.setIndents( indents<wcoord>(10, 20, 30, 40) );
  Painter.setDrawer( Drawer );
  Painter.update();

  CPPUNIT_ASSERT_EQUAL( 10,  Painter.plotRectangle().left() );
  CPPUNIT_ASSERT_EQUAL( 620, Painter.plotRectangle().right() );
  CPPUNIT_ASSERT_EQUAL( 40,  Painter.plotRectangle().down() );
  CPPUNIT_ASSERT_EQUAL( 450, Painter.plotRectangle().up() );

  Drawer->setWidth(100);
  Painter.update();
  CPPUNIT_ASSERT_EQUAL( 10,  Painter.plotRectangle().left() );
  CPPUNIT_ASSERT_EQUAL( 80,  Painter.plotRectangle().right() );
}

// ---------------------------------------------------------

void scigraphics::tests::test_painter::update()
{
  auto Drawer = new mockDrawer();
  painterMorozov Painter( Drawer );
  Painter.update();
  
  CPPUNIT_ASSERT_EQUAL( 640, Painter.plotRectangle().width() );
  CPPUNIT_ASSERT_EQUAL( 480, Painter.plotRectangle().height() );

  Drawer->setWidth( 320 );
  Drawer->setHeight( 240 );
  Painter.update();
  
  CPPUNIT_ASSERT_EQUAL( 320, Painter.plotRectangle().width() );
  CPPUNIT_ASSERT_EQUAL( 240, Painter.plotRectangle().height() );
}

// ---------------------------------------------------------

void scigraphics::tests::test_painter::fpoint2wpoint()
{
  auto Drawer = new mockDrawer();
  painterMorozov Painter( Drawer );
  Painter.setIndents( indents<wcoord>(10, 30, 20, 60) );
  Painter.update();
  
  CPPUNIT_ASSERT_EQUAL( 600, Painter.plotRectangle().width() );
  CPPUNIT_ASSERT_EQUAL( 400, Painter.plotRectangle().height() );
 
  wpoint Point(0,0);

  Point = Painter.fpoint2wpoint( fpoint(0,0) );
  CPPUNIT_ASSERT_EQUAL(  10, Point.x() );
  CPPUNIT_ASSERT_EQUAL( 420, Point.y() );
  
  Point = Painter.fpoint2wpoint( fpoint(0.5,0) );
  CPPUNIT_ASSERT_EQUAL( 310, Point.x() );
  CPPUNIT_ASSERT_EQUAL( 420, Point.y() );
  
  Point = Painter.fpoint2wpoint( fpoint(0,0.5) );
  CPPUNIT_ASSERT_EQUAL(  10, Point.x() );
  CPPUNIT_ASSERT_EQUAL( 220, Point.y() );
  
  Point = Painter.fpoint2wpoint( fpoint(0.1,0.6) );
  CPPUNIT_ASSERT_EQUAL(  70, Point.x() );
  CPPUNIT_ASSERT_EQUAL( 180, Point.y() );
}

// ---------------------------------------------------------

void scigraphics::tests::test_painter::canSeparatePoints()
{
#if 0
  auto Drawer = new mockDrawer();
  painterMorozov Painter( Drawer );
  Painter.update();

  fpoint Pt1( 0.5, 0.25 );
  fpoint Pt2( 0.5, 0.25 - 1e-4 );
  
  CPPUNIT_ASSERT( ! Painter.canSeparateNormPoints(Pt1,Pt1) );
  CPPUNIT_ASSERT( ! Painter.canSeparateNormPoints(Pt1,Pt2) );
  
  Pt2.moveX(0.1);
  CPPUNIT_ASSERT( Painter.canSeparateNormPoints(Pt1,Pt2) );
#endif
}
        
// ---------------------------------------------------------

void scigraphics::tests::test_painter::textWidth()
{
  auto Drawer = new mockDrawer();
  painterMorozov Painter( Drawer );

  textStyle TextStyle;
  TextStyle.setFontSize(10);

  CPPUNIT_ASSERT_EQUAL( Drawer->textWidth("xxx",TextStyle), Painter.textWidth("xxx",TextStyle) );
  CPPUNIT_ASSERT_EQUAL( 30, Painter.textWidth("xxx",TextStyle) ); // Only for mock drawer
}
        
// ---------------------------------------------------------

void scigraphics::tests::test_painter::textHeight()
{
  auto Drawer = new mockDrawer();
  painterMorozov Painter( Drawer );

  textStyle TextStyle;
  TextStyle.setFontSize(10);

  CPPUNIT_ASSERT_EQUAL( Drawer->textHeight("xxx",TextStyle), Painter.textHeight("xxx",TextStyle) );
  CPPUNIT_ASSERT_EQUAL( 10, Painter.textHeight("xxx",TextStyle) ); // Only for mock drawer
}

// =========================================================

