
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





