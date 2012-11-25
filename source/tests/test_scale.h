
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
#include "scale.h"

// =========================================================

namespace scigraphics
{
  namespace tests
  {

// =========================================================

    class test_scaleLinear : public CppUnit::TestFixture
    {
      struct scaleLinearMorozov : public scaleLinear
      {
        friend class test_scaleLinear;
      };

        CPPUNIT_TEST_SUITE( test_scaleLinear );
        CPPUNIT_TEST( numberToPartOfDistance );
        CPPUNIT_TEST( partOfDistanceToNumber );
        CPPUNIT_TEST( numberToFraction );
        CPPUNIT_TEST( numberToFractionZoom );
        CPPUNIT_TEST( numberToFractionShift );
        CPPUNIT_TEST( numberToFractionZoomShift );
        CPPUNIT_TEST( fractionToNumber );
        CPPUNIT_TEST( marks );
        CPPUNIT_TEST_SUITE_END();

        numberLimits createNumberLimits();

      public:
        void numberToPartOfDistance();
        void partOfDistanceToNumber();
        void numberToFraction();
        void numberToFractionZoom();
        void numberToFractionShift();
        void numberToFractionZoomShift();
        void fractionToNumber();
        void marks();
        void setUp() {}
        void tearDown() {}
    };

    // =========================================================
    
    class test_scaleLogPositive : public CppUnit::TestFixture
    {
        CPPUNIT_TEST_SUITE( test_scaleLogPositive );
        CPPUNIT_TEST( numberToFraction );
        CPPUNIT_TEST( fractionToNumber );
        CPPUNIT_TEST( marks );
        CPPUNIT_TEST_SUITE_END();

        numberLimits createNumberLimits();

      public:
        void numberToFraction();
        void fractionToNumber();
        void marks();
        void setUp() {}
        void tearDown() {}
    };

    // =========================================================
    
    class test_scaleLogNegative : public CppUnit::TestFixture
    {
        CPPUNIT_TEST_SUITE( test_scaleLogNegative );
        CPPUNIT_TEST( numberToFraction );
        CPPUNIT_TEST( fractionToNumber );
        CPPUNIT_TEST( marks );
        CPPUNIT_TEST_SUITE_END();

        numberLimits createNumberLimits();

      public:
        void numberToFraction();
        void fractionToNumber();
        void marks();
        void setUp() {}
        void tearDown() {}
    };

    // =========================================================
    
    class test_scaleSquare : public CppUnit::TestFixture
    {
        CPPUNIT_TEST_SUITE( test_scaleSquare );
        CPPUNIT_TEST( numberToFraction );
        CPPUNIT_TEST( fractionToNumber );
        CPPUNIT_TEST( marks );
        CPPUNIT_TEST_SUITE_END();

        numberLimits createNumberLimits();

      public:
        void numberToFraction();
        void fractionToNumber();
        void marks();
        void setUp() {}
        void tearDown() {}
    };

    // =========================================================
    
  }
}



