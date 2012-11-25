
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


#include "test_graph.h"
#include "scigraphics/graph.h"
#include "scigraphics/graphsequence.h"
#include "scigraphics/graphviewcollection.h"

#include <algorithm>
#include <cstring>
#include <memory>
#include <cmath>
#include <typeinfo>

// =========================================================

CPPUNIT_TEST_SUITE_REGISTRATION( scigraphics::tests::test_graphSequenceVector );

// =========================================================

void scigraphics::tests::test_graphSequenceVector::getView()
{
  graphSequenceVector Graph;

  CPPUNIT_ASSERT( Graph.getViews().getView<graphViewLine>() != NULL );
  CPPUNIT_ASSERT_EQUAL( std::string() + typeid(graphViewLine).name(), std::string() + typeid(*Graph.getViews().getView<graphViewLine>()).name() );
}

// ------------------------------------------------------------

void scigraphics::tests::test_graphSequenceVector::setViewVisible()
{
  graphSequenceVector Graph;

  CPPUNIT_ASSERT_EQUAL( true, Graph.getViews().getView<graphViewLine>()->isVisible() );
  
  Graph.setVisibleLines( false );
  CPPUNIT_ASSERT_EQUAL( false, Graph.getViews().getView<graphViewLine>()->isVisible() );
}

// ------------------------------------------------------------

void scigraphics::tests::test_graphSequenceVector::isViewExist()
{
  graphSequenceVector Graph;

  CPPUNIT_ASSERT_EQUAL( true, Graph.getViews().isViewExist<graphViewLine>() );
}

// ------------------------------------------------------------

void scigraphics::tests::test_graphSequenceVector::limitsX()
{
  graphSequenceVector Graph;
  Graph.append( 0, 0 );
  Graph.append( 2, 2 );
  Graph.append( -1, -1 );


  CPPUNIT_ASSERT_DOUBLES_EQUAL( -1, Graph.limitsX().totalLimits().min(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( -1, Graph.limitsX().negativeLimits().max(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 2, Graph.limitsX().positiveLimits().min(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 2, Graph.limitsX().totalLimits().max(), 1e-5 );
}

// ------------------------------------------------------------

void scigraphics::tests::test_graphSequenceVector::limitsY()
{
  graphSequenceVector Graph;
  Graph.append( 0, 0 );
  Graph.append( 2, 2 );
  Graph.append( -1, -1 );
  
  CPPUNIT_ASSERT_DOUBLES_EQUAL( -1, Graph.limitsY( interval<number>(-2,3) ).totalLimits().min(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 2,  Graph.limitsY( interval<number>(-2,3) ).totalLimits().max(), 1e-5 );
}

// ------------------------------------------------------------

void scigraphics::tests::test_graphSequenceVector::legend()
{
  const std::string Legend = "Legend";
  graphSequenceVector Graph( Legend, color::Black );

  CPPUNIT_ASSERT_EQUAL( Legend, Graph.legend() );
  CPPUNIT_ASSERT_EQUAL( true, Graph.showLegend() ); 
  
  Graph.setLegend( Legend + "x" );
  CPPUNIT_ASSERT_EQUAL( Legend + "x", Graph.legend() );

  Graph.setShowLegend(false);
  CPPUNIT_ASSERT_EQUAL( false, Graph.showLegend() );
}

// =========================================================

