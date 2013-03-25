
// =========================================================

#include <algorithm>
#include <cstring>
#include <cmath>

#include <gtest/gtest.h>

#include "color.h"

namespace scigraphics {};
using namespace scigraphics;

// =========================================================

TEST( test_color, hsv )
{
  color C( 200, 20, 50, 10 );
 
  ASSERT_EQ( 200U, C.red() );
  ASSERT_EQ( 20U,  C.green() );
  ASSERT_EQ( 50U,  C.blue() );
  ASSERT_EQ( 10U,  C.transparency() );

  ASSERT_EQ( 350U, C.hue() );
  ASSERT_EQ( 90U, C.saturation() );
  ASSERT_EQ( 78U, C.value() );

  color C2 = color::fromHSV( 200, 50, 20, 15 );
  ASSERT_NEAR( 26U, C2.red(), 1.1 );
  ASSERT_NEAR( 43U, C2.green(), 1.1 );
  ASSERT_NEAR( 51U, C2.blue(), 1.1 );
  ASSERT_EQ( 15U, C2.transparency() );
}

// ---------------------------------------------------------

TEST( test_color, transparency )
{
  color C = color::Magenta;

  ASSERT_EQ( 0U, C.transparency() );

  C.setTransparency( 0.2 );
  ASSERT_NEAR( (0.2*255), C.transparency(), 1 );
}

// =========================================================

