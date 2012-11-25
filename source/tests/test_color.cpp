
/*
 * Copyright 2011,2012 Evgeniy Khilkevitch 
 * 
 * This file is part of scigraphics.
 * 
 * Scigraphics is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * Scigraphics is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with scigraphics.  If not, see <http://www.gnu.org/licenses/>.
 */


// =========================================================

#include "test_color.h"
#include "color.h"

#include <algorithm>
#include <cstring>
#include <memory>
#include <cmath>

// =========================================================

CPPUNIT_TEST_SUITE_REGISTRATION( scigraphics::tests::test_color );

// =========================================================

void scigraphics::tests::test_color::hsv()
{
  color C( 200, 20, 50, 10 );
 
  CPPUNIT_ASSERT_EQUAL( 200U, C.red() );
  CPPUNIT_ASSERT_EQUAL( 20U,  C.green() );
  CPPUNIT_ASSERT_EQUAL( 50U,  C.blue() );
  CPPUNIT_ASSERT_EQUAL( 10U,  C.transparency() );

  CPPUNIT_ASSERT_EQUAL( 350U, C.hue() );
  CPPUNIT_ASSERT_EQUAL( 90U, C.saturation() );
  CPPUNIT_ASSERT_EQUAL( 78U, C.value() );

  color C2 = color::fromHSV( 200, 50, 20, 15 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 26U, C2.red(), 1.1 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 43U, C2.green(), 1.1 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 51U, C2.blue(), 1.1 );
  CPPUNIT_ASSERT_EQUAL( 15U, C2.transparency() );
}

// ---------------------------------------------------------

void scigraphics::tests::test_color::transparency()
{
  color C = color::Magenta;

  CPPUNIT_ASSERT_EQUAL( 0U, C.transparency() );

  C.setTransparency( 0.2 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( (0.2*255), C.transparency(), 1 );
}

// =========================================================

