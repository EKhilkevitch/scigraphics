
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

