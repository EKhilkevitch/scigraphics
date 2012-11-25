
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


#pragma once

#include <cppunit/extensions/HelperMacros.h>

// =========================================================

namespace scigraphics
{
  class axisSet;

  namespace tests
  {

// =========================================================

    class test_plotLimits_limitsXY : public CppUnit::TestFixture
    {
        CPPUNIT_TEST_SUITE( test_plotLimits_limitsXY );
        CPPUNIT_TEST( isAxisSetXY );
        CPPUNIT_TEST( set );
        CPPUNIT_TEST( applyStretch );
        CPPUNIT_TEST_SUITE_END();

      public:
        void isAxisSetXY();
        void set();
        void applyStretch();
        void setUp() {}
        void tearDown() {}
    };

    class test_plotLimits : public CppUnit::TestFixture
    {
        axisSet *AxisX, *AxisY;

        CPPUNIT_TEST_SUITE( test_plotLimits );
        CPPUNIT_TEST( limitsForEmptyGraphics );
        CPPUNIT_TEST( limitsForGraphics );
        CPPUNIT_TEST( limitsForGraphicsPositive );
        CPPUNIT_TEST( limitsForGraphicsNegative );
        CPPUNIT_TEST( limitsForGraphicsCloseToZero );
        CPPUNIT_TEST( limitsForGraphicsStretch );
        CPPUNIT_TEST_SUITE_END();

      public:
        void limitsForEmptyGraphics();
        void limitsForGraphics();
        void limitsForGraphicsPositive();
        void limitsForGraphicsNegative();
        void limitsForGraphicsCloseToZero();
        void limitsForGraphicsStretch();
        void setUp();
        void tearDown();
    };

    // =========================================================
    
  }
}





