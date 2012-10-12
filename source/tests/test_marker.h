
#pragma once

#include <cppunit/extensions/HelperMacros.h>
#include "marker.h"

// =========================================================

namespace graphics
{
  namespace tests
  {

// =========================================================

    class test_markerLinear : public CppUnit::TestFixture
    {
        CPPUNIT_TEST_SUITE( test_markerLinear );
        CPPUNIT_TEST( marks );
        CPPUNIT_TEST_SUITE_END();
        
      public:
        void marks();
        void setUp() {}
        void tearDown() {}
    };

    // =========================================================
    
    class test_markerLogPositive : public CppUnit::TestFixture
    {
        CPPUNIT_TEST_SUITE( test_markerLogPositive );
        CPPUNIT_TEST( marks );
        CPPUNIT_TEST_SUITE_END();
        
      public:
        void marks();
        void setUp() {}
        void tearDown() {}
    };

    // =========================================================
    
    class test_markerLogNegative : public CppUnit::TestFixture
    {
        CPPUNIT_TEST_SUITE( test_markerLogNegative );
        CPPUNIT_TEST( marks );
        CPPUNIT_TEST_SUITE_END();
        
      public:
        void marks();
        void setUp() {}
        void tearDown() {}
    };

    // =========================================================
    

  }
}





