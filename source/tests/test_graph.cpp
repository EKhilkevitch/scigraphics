
#include "test_graph.h"
#include "graphics/graph.h"
#include "graphics/graphsequence.h"

#include <algorithm>
#include <cstring>
#include <memory>
#include <cmath>
#include <typeinfo>

// =========================================================

CPPUNIT_TEST_SUITE_REGISTRATION( graphics::tests::test_graphSequenceVector );

// =========================================================

void graphics::tests::test_graphSequenceVector::getView()
{
  graphSequenceVector Graph;

  CPPUNIT_ASSERT( Graph.getViews().getView<graphViewLine>() != NULL );
  CPPUNIT_ASSERT_EQUAL( std::string() + typeid(graphViewLine).name(), std::string() + typeid(*Graph.getViews().getView<graphViewLine>()).name() );
}

// ------------------------------------------------------------

void graphics::tests::test_graphSequenceVector::setViewVisible()
{
  graphSequenceVector Graph;

  CPPUNIT_ASSERT_EQUAL( true, Graph.getViews().getView<graphViewLine>()->isVisible() );
  
  Graph.setVisibleLines( false );
  CPPUNIT_ASSERT_EQUAL( false, Graph.getViews().getView<graphViewLine>()->isVisible() );
}

// ------------------------------------------------------------

void graphics::tests::test_graphSequenceVector::isViewExist()
{
  graphSequenceVector Graph;

  CPPUNIT_ASSERT_EQUAL( true, Graph.getViews().isViewExist<graphViewLine>() );
}

// ------------------------------------------------------------

void graphics::tests::test_graphSequenceVector::limitsX()
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

void graphics::tests::test_graphSequenceVector::limitsY()
{
  graphSequenceVector Graph;
  Graph.append( 0, 0 );
  Graph.append( 2, 2 );
  Graph.append( -1, -1 );
  
  CPPUNIT_ASSERT_DOUBLES_EQUAL( -1, Graph.limitsY( interval<number>(-2,3) ).totalLimits().min(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 2,  Graph.limitsY( interval<number>(-2,3) ).totalLimits().max(), 1e-5 );
}

// ------------------------------------------------------------

void graphics::tests::test_graphSequenceVector::legend()
{
  const std::string Legend = "Legend";
  graphSequenceVector Graph( Legend );

  CPPUNIT_ASSERT_EQUAL( Legend, Graph.legend() );
  CPPUNIT_ASSERT_EQUAL( true, Graph.showLegend() ); 
  
  Graph.setLegend( Legend + "x" );
  CPPUNIT_ASSERT_EQUAL( Legend + "x", Graph.legend() );

  Graph.setShowLegend(false);
  CPPUNIT_ASSERT_EQUAL( false, Graph.showLegend() );
}

// =========================================================

