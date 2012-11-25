
/*
 * Copyright 2011,2012 Evgeniy Khilkevitch 
 * 
 * This file is part of scigraphics.
 * 
 * Scigraphics is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * Scigraphics is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with scigraphics.  If not, see <http://www.gnu.org/licenses/>.
 */


#pragma once

#include <cppunit/extensions/HelperMacros.h>

#include "legend.h"

// =========================================================

namespace scigraphics
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


