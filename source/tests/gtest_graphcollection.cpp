
// =========================================================

#include <gtest/gtest.h>

#include "scigraphics/graphcollection.h"
#include "scigraphics/graphsequence.h"
#include "scigraphics/axisset.h"

#include <list>

using namespace scigraphics;

// =========================================================

TEST( test_graphCollection, scales )
{
  graphCollection Collection;
  
  ASSERT_EQ( (const scigraphics::scale*)NULL, Collection.graphScaleX(NULL) );
  ASSERT_EQ( (const scigraphics::scale*)NULL, Collection.graphScaleY(NULL) );


  axisSet *AxisX = new axisSetX(0);
  axisSet *AxisY = new axisSetY(0);
  Collection.setDefaultAxisSets(AxisX,AxisY);
 
  graph *Graph = Collection.create<sequence::graphVector>(color());
  
  ASSERT_EQ( (const scigraphics::scale*)AxisX->getScale(), Collection.graphScaleX(Graph) );
  ASSERT_EQ( (const scigraphics::scale*)AxisY->getScale(), Collection.graphScaleY(Graph) );
  ASSERT_EQ( (const scigraphics::axisSet*)AxisX, Collection.graphAxisSetX(Graph) );
  ASSERT_EQ( (const scigraphics::axisSet*)AxisY, Collection.graphAxisSetY(Graph) );

  delete AxisX;
  delete AxisY;
}

// ---------------------------------------------------------

TEST( test_graphCollection, bindGraphToAxisSet )
{
  graphCollection Collection;
  axisSetX SetX(0);
  axisSetY SetY(0);
  Collection.setDefaultAxisSets( &SetX, &SetY );

  graph *Graph = Collection.create<sequence::graphVector>(color());
  ASSERT_EQ( (const scigraphics::scale*)(SetX.getScale()), Collection.graphScaleX(Graph) );
  ASSERT_EQ( (const scigraphics::scale*)(SetY.getScale()), Collection.graphScaleY(Graph) );

  axisSet *AxisX = new axisSetX(0);
  axisSet *AxisY = new axisSetY(0);
  Collection.bindGraphToAxisSet( Graph, AxisX, AxisY );

  ASSERT_EQ( (const scigraphics::scale*)AxisX->getScale(), Collection.graphScaleX(Graph) );
  ASSERT_EQ( (const scigraphics::scale*)AxisY->getScale(), Collection.graphScaleY(Graph) );

  delete AxisX;
  delete AxisY;
}

// ---------------------------------------------------------

TEST( test_graphCollection, append )
{
  graphCollection Collection;
  axisSetX SetX(0);
  axisSetY SetY(0);
  Collection.setDefaultAxisSets( &SetX, &SetY );

  ASSERT_TRUE( Collection.empty() );
  ASSERT_EQ( (size_t)0, Collection.size() );

  graph *Graph = new sequence::graphVector(color::Black);

  Collection.append(Graph);
  
  ASSERT_TRUE( ! Collection.empty() );
  ASSERT_EQ( (size_t)1, Collection.size() );
  ASSERT_TRUE( Collection.begin() != Collection.end() );
  
  Graph->setLegend("1");
  ASSERT_EQ( std::string("1"), Collection.begin()->legend() );
  
  Collection.append( new sequence::graphVector(color::Black) );
  ASSERT_EQ( (size_t)2, Collection.size() );

  try
  {
    Collection.append(nullptr);
    FAIL() << "NULL can not be append to collection";;
  } catch ( std::exception ) {}
  
  try
  {
    Collection.append(Graph);
    FAIL() << "Graph already exist in collection";;
  } catch ( std::exception ) {}
  
  ASSERT_EQ( (size_t)2, Collection.size() );
}

// ---------------------------------------------------------

TEST( test_graphCollection, create )
{
  graphCollection Collection;
  axisSetX SetX(0);
  axisSetY SetY(0);
  Collection.setDefaultAxisSets( &SetX, &SetY );

  auto Graph = Collection.create<sequence::graphVector>("L",color::Black);

  ASSERT_EQ( (size_t)1, Collection.size() );
  ASSERT_EQ( std::string("L"), Graph->legend() );
  ASSERT_EQ( std::string("L"), Collection.begin()->legend() );
  
  auto Graph2 = Collection.create<sequence::graphVector>(color());

  ASSERT_EQ( (size_t)2, Collection.size() );
  ASSERT_EQ( std::string(), Graph2->legend() );
  ASSERT_TRUE( Graph != Graph2 );
  
  try
  {
    Collection.append(Graph);
    FAIL() << "Graph already exist in collection";;
  } catch ( std::exception ) {}
}

// ---------------------------------------------------------

TEST( test_graphCollection, clear )
{
  graphCollection Collection;
  axisSetX SetX(0);
  axisSetY SetY(0);
  Collection.setDefaultAxisSets( &SetX, &SetY );

  ASSERT_TRUE( Collection.empty() );

  Collection.clear();
  ASSERT_TRUE( Collection.empty() );
  
  Collection.create<sequence::graphVector>(color());
  ASSERT_TRUE( ! Collection.empty() );
  Collection.clear();
  ASSERT_TRUE( Collection.empty() );
}

// ---------------------------------------------------------

TEST( test_graphCollection, exist )
{
  graphCollection Collection;
  axisSetX SetX(0);
  axisSetY SetY(0);
  Collection.setDefaultAxisSets( &SetX, &SetY );

  graph *Graph = new sequence::graphVector(color::Black);
  ASSERT_TRUE( ! Collection.exists(NULL) );
  ASSERT_TRUE( ! Collection.exists(Graph) );

  Collection.append( Graph );
  ASSERT_TRUE( Collection.exists(Graph) );
}

// ---------------------------------------------------------

TEST( test_graphCollection, iterators )
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
    Collection.create<sequence::graphVector>( *l, color() );

  ASSERT_EQ( Legends.size(), Collection.size() );

  auto li = Legends.begin();
  auto gi = Collection.begin();

  for ( ; li != Legends.end(); ++li, ++gi )
    ASSERT_EQ( *li, gi->legend() );
}

// =========================================================

