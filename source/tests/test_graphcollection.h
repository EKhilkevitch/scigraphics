
#pragma once

#include <cppunit/extensions/HelperMacros.h>

// =========================================================

namespace scigraphics
{
  class axisSet;

  namespace tests
  {

// =========================================================

    class test_graphCollection : public CppUnit::TestFixture
    {
      private:
        CPPUNIT_TEST_SUITE( test_graphCollection );
        CPPUNIT_TEST( append );
        CPPUNIT_TEST( create );
        CPPUNIT_TEST( clear );
        CPPUNIT_TEST( exist );
        CPPUNIT_TEST( iterators );
        CPPUNIT_TEST( scales );
        CPPUNIT_TEST( bindGraphToAxisSet );
        CPPUNIT_TEST_SUITE_END();

      public:
        void append();
        void create();
        void clear();
        void exist(); 
        void iterators();
        void scales();
        void bindGraphToAxisSet();
        void setUp() {};
        void tearDown() {};
    };

    // =========================================================
    
  }
}


