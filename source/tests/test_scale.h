
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



