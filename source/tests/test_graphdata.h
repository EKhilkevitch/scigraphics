
#pragma once

#include <cppunit/extensions/HelperMacros.h>

// =========================================================

namespace scigraphics
{
  namespace tests
  {

// =========================================================

    class test_dataSequenceVector : public CppUnit::TestFixture
    {
        CPPUNIT_TEST_SUITE( test_dataSequenceVector );
        CPPUNIT_TEST( point );
        CPPUNIT_TEST( append );
        CPPUNIT_TEST( iterator );
        CPPUNIT_TEST( limitsX );
        CPPUNIT_TEST( limitsY );
        CPPUNIT_TEST( clear );
        CPPUNIT_TEST( isOrderedByX );
        CPPUNIT_TEST_SUITE_END();

      public:
        void point();
        void append();
        void iterator();
        void limitsX();
        void limitsY();
        void clear();
        void isOrderedByX();
        void setUp() {}
        void tearDown() {}
    };

    // =========================================================
    
    class test_dataMap : public CppUnit::TestFixture
    {
        CPPUNIT_TEST_SUITE( test_dataMap );
        CPPUNIT_TEST( set );
        CPPUNIT_TEST( limits );
        CPPUNIT_TEST( resize );
        CPPUNIT_TEST_SUITE_END();

      public:
        void set();
        void limits();
        void resize();

        void setUp() {}
        void tearDown() {}
    };
    
    // =========================================================
    
  }
}

