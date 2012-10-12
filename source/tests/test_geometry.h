
#pragma once

#include <cppunit/extensions/HelperMacros.h>

// =========================================================

namespace graphics
{
  namespace tests
  {

// =========================================================

    class test_geometry : public CppUnit::TestFixture
    {
        CPPUNIT_TEST_SUITE( test_geometry );
        CPPUNIT_TEST( tpoint );
        CPPUNIT_TEST( trectangle );
        CPPUNIT_TEST( tindents );
        CPPUNIT_TEST_SUITE_END();

      public:
        void tpoint();
        void trectangle();
        void tindents();
        void setUp() {}
        void tearDown() {}
    };

    // =========================================================
    
  }
}

