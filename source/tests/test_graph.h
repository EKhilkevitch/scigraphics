
#pragma once

#include <cppunit/extensions/HelperMacros.h>

// =========================================================

namespace graphics
{
  namespace tests
  {

// =========================================================

    class test_graphSequenceVector : public CppUnit::TestFixture
    {
        CPPUNIT_TEST_SUITE( test_graphSequenceVector );
        CPPUNIT_TEST( getView );
        CPPUNIT_TEST( setViewVisible );
        CPPUNIT_TEST( isViewExist );
        CPPUNIT_TEST( limitsX );
        CPPUNIT_TEST( limitsY );
        CPPUNIT_TEST( legend );
        CPPUNIT_TEST_SUITE_END();

      public:
        void getView();
        void setViewVisible();
        void isViewExist();
        void limitsX();
        void limitsY();
        void legend();
        void setUp() {}
        void tearDown() {}
    };

    // =========================================================
    
  }
}



