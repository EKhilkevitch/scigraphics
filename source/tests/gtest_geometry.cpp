
// =========================================================

#include <gtest/gtest.h>

#include "geometry.h"

namespace scigraphics {};
using namespace scigraphics;

// =========================================================

TEST( test_geometry, tpoint )
{
  point<number> Point( 1, 2 );

  ASSERT_NEAR( 1, Point.x(), 1e-5 );
  ASSERT_NEAR( 2, Point.y(), 1e-5 );

  Point.moveX( 0.5 );
  ASSERT_NEAR( 1.5, Point.x(), 1e-5 );
  ASSERT_NEAR( 2.0, Point.y(), 1e-5 );
  
  Point.moveY( -0.5 );
  ASSERT_NEAR( 1.5, Point.x(), 1e-5 );
  ASSERT_NEAR( 1.5, Point.y(), 1e-5 );

  point<number> Pt1( 1, 2 ), Pt2( 1, 2 );
  ASSERT_TRUE( Pt1 == Pt2 );
  Pt1.moveX(0.1);
  ASSERT_TRUE( Pt1 != Pt2 );

  ASSERT_TRUE( point<number>::zero() == point<number>(0,0) );
}

// ---------------------------------------------------------

TEST( test_geometry, trectangle )
{
  rectangle<number> Rect( point<number>(6,2), point<number>(1,8) );

  ASSERT_NEAR( 1, Rect.left(),  1e-5 );
  ASSERT_NEAR( 6, Rect.right(), 1e-5 );
  ASSERT_NEAR( 8, Rect.up(),    1e-5 );
  ASSERT_NEAR( 2, Rect.down(),  1e-5 );
  ASSERT_NEAR( 5, Rect.width(), 1e-5 );
  ASSERT_NEAR( 6, Rect.height(),1e-5 );
  ASSERT_TRUE( Rect.contain( point<number>(2,3) ) );
  ASSERT_TRUE( Rect.contain( point<number>(1,2) ) );
  ASSERT_TRUE( ! Rect.contain( point<number>(0,3) ) );
  ASSERT_TRUE( ! Rect.contain( point<number>(0,0) ) );
 
  Rect.moveX( 0.5 );
  ASSERT_NEAR( 1.5, Rect.left(),  1e-5 );
  ASSERT_NEAR( 6.5, Rect.right(), 1e-5 );
  ASSERT_NEAR( 8.0, Rect.up(),    1e-5 );
  ASSERT_NEAR( 2.0, Rect.down(),  1e-5 );
  
  Rect.moveY( -0.5 );
  ASSERT_NEAR( 1.5, Rect.left(),  1e-5 );
  ASSERT_NEAR( 6.5, Rect.right(), 1e-5 );
  ASSERT_NEAR( 7.5, Rect.up(),    1e-5 );
  ASSERT_NEAR( 1.5, Rect.down(),  1e-5 );
}

// ---------------------------------------------------------

TEST( test_geometry, tindents )
{
  indents<number> Indents( 0.5, 1.5, 2, 3 );
  rectangle<number> Rect( point<number>(6,2), point<number>(1,8) );
  rectangle<number> IRect = Indents.apply(Rect);

  ASSERT_NEAR( 1.5, IRect.left(),  1e-5 );
  ASSERT_NEAR( 4.5, IRect.right(), 1e-5 );
  ASSERT_NEAR( 6, IRect.up(),    1e-5 );
  ASSERT_NEAR( 5, IRect.down(),  1e-5 );

}

// =========================================================

