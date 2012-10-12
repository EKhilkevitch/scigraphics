
#pragma once

#include <cppunit/extensions/HelperMacros.h>

// =========================================================

namespace graphics
{
  namespace tests
  {

// =========================================================

    class test_color : public CppUnit::TestFixture
    {
        CPPUNIT_TEST_SUITE( test_color );
        CPPUNIT_TEST( hsv );
        CPPUNIT_TEST( transparency );
        CPPUNIT_TEST_SUITE_END();

      public:
        void hsv();
        void transparency();
        void setUp() {}
        void tearDown() {}
    };

// =========================================================
    
  }
}

