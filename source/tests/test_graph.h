
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



