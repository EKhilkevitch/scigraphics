
#pragma once

#include <cppunit/extensions/HelperMacros.h>

// =========================================================

namespace scigraphics
{
  namespace tests
  {

// =========================================================

    class test_numberLimits : public CppUnit::TestFixture
    {
        CPPUNIT_TEST_SUITE( test_numberLimits );
        CPPUNIT_TEST( updateLimitsPosNums );
        CPPUNIT_TEST( updateLimitsNegNums );
        CPPUNIT_TEST( updateLimitsZero );
        CPPUNIT_TEST( updateLimits );
        CPPUNIT_TEST( isValidInterval );
        CPPUNIT_TEST( clear );
        CPPUNIT_TEST_SUITE_END();

      public:
        void isValidInterval();
        void clear();
        void updateLimitsPosNums();
        void updateLimitsNegNums();
        void updateLimitsZero();
        void updateLimits();
        void setUp() {}
        void tearDown() {}
    };

    // =========================================================
    
  }
}



