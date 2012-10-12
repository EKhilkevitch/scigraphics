
// =========================================================

#include "test_geometry.h"
#include "geometry.h"

// =========================================================

CPPUNIT_TEST_SUITE_REGISTRATION( scigraphics::tests::test_geometry );

// =========================================================

void scigraphics::tests::test_geometry::tpoint()
{
  point<number> Point( 1, 2 );

  CPPUNIT_ASSERT_DOUBLES_EQUAL( 1, Point.x(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 2, Point.y(), 1e-5 );

  Point.moveX( 0.5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.5, Point.x(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 2.0, Point.y(), 1e-5 );
  
  Point.moveY( -0.5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.5, Point.x(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.5, Point.y(), 1e-5 );

  point<number> Pt1( 1, 2 ), Pt2( 1, 2 );
  CPPUNIT_ASSERT( Pt1 == Pt2 );
  Pt1.moveX(0.1);
  CPPUNIT_ASSERT( Pt1 != Pt2 );

  CPPUNIT_ASSERT( point<number>::zero() == point<number>(0,0) );
}

// ---------------------------------------------------------

void scigraphics::tests::test_geometry::trectangle()
{
  rectangle<number> Rect( point<number>(6,2), point<number>(1,8) );

  CPPUNIT_ASSERT_DOUBLES_EQUAL( 1, Rect.left(),  1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 6, Rect.right(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 8, Rect.up(),    1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 2, Rect.down(),  1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 5, Rect.width(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 6, Rect.height(),1e-5 );
  CPPUNIT_ASSERT( Rect.contain( point<number>(2,3) ) );
  CPPUNIT_ASSERT( Rect.contain( point<number>(1,2) ) );
  CPPUNIT_ASSERT( ! Rect.contain( point<number>(0,3) ) );
  CPPUNIT_ASSERT( ! Rect.contain( point<number>(0,0) ) );
 
  Rect.moveX( 0.5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.5, Rect.left(),  1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 6.5, Rect.right(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 8.0, Rect.up(),    1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 2.0, Rect.down(),  1e-5 );
  
  Rect.moveY( -0.5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.5, Rect.left(),  1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 6.5, Rect.right(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 7.5, Rect.up(),    1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.5, Rect.down(),  1e-5 );
}

// ---------------------------------------------------------

void scigraphics::tests::test_geometry::tindents()
{
  indents<number> Indents( 0.5, 1.5, 2, 3 );
  rectangle<number> Rect( point<number>(6,2), point<number>(1,8) );
  rectangle<number> IRect = Indents.apply(Rect);

  CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.5, IRect.left(),  1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 4.5, IRect.right(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 6, IRect.up(),    1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 5, IRect.down(),  1e-5 );

}

// =========================================================

