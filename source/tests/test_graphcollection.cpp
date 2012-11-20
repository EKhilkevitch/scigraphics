
// =========================================================

#include "test_graphcollection.h"
#include "scigraphics/graphcollection.h"
#include "scigraphics/graphsequence.h"
#include "scigraphics/axisset.h"

// =========================================================

CPPUNIT_TEST_SUITE_REGISTRATION( scigraphics::tests::test_graphCollection );

// =========================================================

void scigraphics::tests::test_graphCollection::scales()
{
  graphCollection Collection;
  
  CPPUNIT_ASSERT_EQUAL( (const scigraphics::scale*)NULL, Collection.graphScaleX(NULL) );
  CPPUNIT_ASSERT_EQUAL( (const scigraphics::scale*)NULL, Collection.graphScaleY(NULL) );


  axisSet *AxisX = new axisSetX(0);
  axisSet *AxisY = new axisSetY(0);
  Collection.setDefaultAxisSets(AxisX,AxisY);
 
  graph *Graph = Collection.create<graphSequenceVector>(color());
  
  CPPUNIT_ASSERT_EQUAL( (const scigraphics::scale*)AxisX->getScale(), Collection.graphScaleX(Graph) );
  CPPUNIT_ASSERT_EQUAL( (const scigraphics::scale*)AxisY->getScale(), Collection.graphScaleY(Graph) );
  CPPUNIT_ASSERT_EQUAL( (const scigraphics::axisSet*)AxisX, Collection.graphAxisSetX(Graph) );
  CPPUNIT_ASSERT_EQUAL( (const scigraphics::axisSet*)AxisY, Collection.graphAxisSetY(Graph) );

  delete AxisX;
  delete AxisY;
}

// ---------------------------------------------------------

void scigraphics::tests::test_graphCollection::bindGraphToAxisSet()
{
  graphCollection Collection;
  axisSetX SetX(0);
  axisSetY SetY(0);
  Collection.setDefaultAxisSets( &SetX, &SetY );

  graph *Graph = Collection.create<graphSequenceVector>(color());
  CPPUNIT_ASSERT_EQUAL( (const scigraphics::scale*)(SetX.getScale()), Collection.graphScaleX(Graph) );
  CPPUNIT_ASSERT_EQUAL( (const scigraphics::scale*)(SetY.getScale()), Collection.graphScaleY(Graph) );

  axisSet *AxisX = new axisSetX(0);
  axisSet *AxisY = new axisSetY(0);
  Collection.bindGraphToAxisSet( Graph, AxisX, AxisY );

  CPPUNIT_ASSERT_EQUAL( (const scigraphics::scale*)AxisX->getScale(), Collection.graphScaleX(Graph) );
  CPPUNIT_ASSERT_EQUAL( (const scigraphics::scale*)AxisY->getScale(), Collection.graphScaleY(Graph) );

  delete AxisX;
  delete AxisY;
}

// ---------------------------------------------------------

void scigraphics::tests::test_graphCollection::append()
{
  graphCollection Collection;
  axisSetX SetX(0);
  axisSetY SetY(0);
  Collection.setDefaultAxisSets( &SetX, &SetY );

  CPPUNIT_ASSERT( Collection.empty() );
  CPPUNIT_ASSERT_EQUAL( (size_t)0, Collection.size() );

  graph *Graph = new graphSequenceVector(color::Black);

  Collection.append(Graph);
  
  CPPUNIT_ASSERT( ! Collection.empty() );
  CPPUNIT_ASSERT_EQUAL( (size_t)1, Collection.size() );
  CPPUNIT_ASSERT( Collection.begin() != Collection.end() );
  
  Graph->setLegend("1");
  CPPUNIT_ASSERT_EQUAL( std::string("1"), (*Collection.begin())->legend() );
  
  Collection.append( new graphSequenceVector(color::Black) );
  CPPUNIT_ASSERT_EQUAL( (size_t)2, Collection.size() );

  try
  {
    Collection.append(nullptr);
    CPPUNIT_FAIL("NULL can not be append to collection");
  } catch ( std::exception ) {}
  
  try
  {
    Collection.append(Graph);
    CPPUNIT_FAIL("Graph already exist in collection");
  } catch ( std::exception ) {}
  
  CPPUNIT_ASSERT_EQUAL( (size_t)2, Collection.size() );
}

// ---------------------------------------------------------

void scigraphics::tests::test_graphCollection::create()
{
  graphCollection Collection;
  axisSetX SetX(0);
  axisSetY SetY(0);
  Collection.setDefaultAxisSets( &SetX, &SetY );

  auto Graph = Collection.create<graphSequenceVector>("L",color::Black);

  CPPUNIT_ASSERT_EQUAL( (size_t)1, Collection.size() );
  CPPUNIT_ASSERT_EQUAL( std::string("L"), Graph->legend() );
  CPPUNIT_ASSERT_EQUAL( std::string("L"), (*Collection.begin())->legend() );
  
  auto Graph2 = Collection.create<graphSequenceVector>(color());

  CPPUNIT_ASSERT_EQUAL( (size_t)2, Collection.size() );
  CPPUNIT_ASSERT_EQUAL( std::string(), Graph2->legend() );
  CPPUNIT_ASSERT( Graph != Graph2 );
  
  try
  {
    Collection.append(Graph);
    CPPUNIT_FAIL("Graph already exist in collection");
  } catch ( std::exception ) {}
}

// ---------------------------------------------------------

void scigraphics::tests::test_graphCollection::clear()
{
  graphCollection Collection;
  axisSetX SetX(0);
  axisSetY SetY(0);
  Collection.setDefaultAxisSets( &SetX, &SetY );

  CPPUNIT_ASSERT( Collection.empty() );

  Collection.clear();
  CPPUNIT_ASSERT( Collection.empty() );
  
  Collection.create<graphSequenceVector>(color());
  CPPUNIT_ASSERT( ! Collection.empty() );
  Collection.clear();
  CPPUNIT_ASSERT( Collection.empty() );
}

// ---------------------------------------------------------

void scigraphics::tests::test_graphCollection::exist()
{
  graphCollection Collection;
  axisSetX SetX(0);
  axisSetY SetY(0);
  Collection.setDefaultAxisSets( &SetX, &SetY );

  graph *Graph = new graphSequenceVector(color::Black);
  CPPUNIT_ASSERT( ! Collection.exist(NULL) );
  CPPUNIT_ASSERT( ! Collection.exist(Graph) );

  Collection.append( Graph );
  CPPUNIT_ASSERT( Collection.exist(Graph) );
}

// ---------------------------------------------------------

void scigraphics::tests::test_graphCollection::iterators()
{
  graphCollection Collection;
  axisSetX SetX(0);
  axisSetY SetY(0);
  Collection.setDefaultAxisSets( &SetX, &SetY );

  std::list< std::string > Legends;
  Legends.push_back("1");
  Legends.push_back("2");
  Legends.push_back("3");

  for ( auto l = Legends.begin(); l != Legends.end(); ++l )
    Collection.create<graphSequenceVector>( *l, color() );

  CPPUNIT_ASSERT_EQUAL( Legends.size(), Collection.size() );

  auto li = Legends.begin();
  auto gi = Collection.begin();

  for ( ; li != Legends.end(); ++li, ++gi )
    CPPUNIT_ASSERT_EQUAL( *li, (*gi)->legend() );
}

// =========================================================


