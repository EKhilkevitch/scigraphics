
#pragma once

#include <cppunit/extensions/HelperMacros.h>

#include "legend.h"

// =========================================================

namespace graphics
{
  namespace tests
  {

// =========================================================

    class test_legend : public CppUnit::TestFixture
    {
      struct legendMorozov : public legend
      {
        friend class test_legend;
      };

        painter Painter;

        CPPUNIT_TEST_SUITE( test_legend );
        CPPUNIT_TEST( shouldDrawGraphLegend );
        CPPUNIT_TEST( legendsList );
        CPPUNIT_TEST( sizesForLegendRectangle );
        CPPUNIT_TEST( updateLegendRectangleShortList );
        CPPUNIT_TEST( updateLegendRectangleLongList );
        CPPUNIT_TEST( updateLegendRectangleVeryLongList );
        CPPUNIT_TEST_SUITE_END();

      public:
        void shouldDrawGraphLegend();
        void legendsList();
        void sizesForLegendRectangle();
        void updateLegendRectangleShortList();
        void updateLegendRectangleLongList();
        void updateLegendRectangleVeryLongList();
        void setUp();
        void tearDown() {}
    };

    // =========================================================
    
  }
}


